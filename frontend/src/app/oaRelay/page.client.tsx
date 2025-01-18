"use client";

import { ArrowUp } from "lucide-react";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { useCallback, useEffect, useRef, useState } from "react";
import { SSE } from "sse.js";
import DOMPurify from "isomorphic-dompurify";
import { marked } from "marked";
import styles from "./page.module.css";
import { useRouter } from "next/navigation";

interface Message {
  role: string;
  content: string;
}

type CurrentMessage = Message & { finished: boolean };

export default function OaRelayClient() {
  const [messages, setMessages] = useState<Message[]>([]);
  const inputRef = useRef<HTMLInputElement>(null);
  const [currentMessage, setCurrentMessage] = useState<
    CurrentMessage | undefined
  >(undefined);
  const [loaded, setLoaded] = useState(false);
  const router = useRouter();

  useEffect(() => {
    async function initialLoad() {
      const res = await fetch("oaRelay/isLoggedIn");
      if (!res.ok) {
        router.push("/oaRelay/login");
        return;
      }

      // User is logged in, loads current messages
      setLoaded(true);
      const msgRes = await fetch("oaRelay/getMessages");
      const msgJson = await msgRes.json();
      setMessages(msgJson.messages);
    }
    initialLoad();
  }, [router]);

  const sendMessage = useCallback(
    (message: string) => {
      setMessages([...messages, { role: "user", content: message }]);
      const source = new SSE("/oaRelay/completions", {
        headers: {
          "Content-Type": "application/json",
        },
        payload: JSON.stringify({
          message: message,
        }),
      });
      source.addEventListener("message", (e: { data: string }) => {
        if (e.data === "[STOP]") {
          setCurrentMessage((orig) => {
            if (!orig) {
              return undefined;
            }
            return {
              ...orig,
              finished: true,
            };
          });
          return;
        }
        const data = JSON.parse(e.data);
        setCurrentMessage((orig) => {
          const delta = data.choices[0].delta;
          if (!orig) {
            return delta;
          }
          return {
            content: orig.content + (delta.content ?? ""),
            role: orig.role + (delta.role ?? ""),
          };
        });
      });
    },
    [messages],
  );

  useEffect(() => {
    if (!currentMessage) {
      return;
    }
    if (currentMessage.finished) {
      setMessages((orig) => [
        ...orig,
        { role: currentMessage.role, content: currentMessage.content },
      ]);
      setCurrentMessage(undefined);
    }
  }, [currentMessage]);

  const markdowns = messages.map((message) => {
    return {
      role: message.role,
      content: DOMPurify.sanitize(marked.parse(message.content) as string),
    };
  });
  const currentMarkdown = DOMPurify.sanitize(
    marked.parse(currentMessage?.content ?? "") as string,
  );

  return (
    <>
      {!loaded && <div className="h-screen w-screen text-center">加载中</div>}
      {loaded && (
        <main className="flex flex-grow flex-col space-y-4 p-4">
          <div className="flex-grow space-y-4 overflow-y-scroll">
            {markdowns.map((markdown, index) => {
              if (markdown.role === "system") {
                return <></>;
              } else {
                return (
                  <div
                    key={index}
                    className={
                      markdown.role === "user"
                        ? styles.userMessage
                        : styles.gptMessage
                    }
                  >
                    <div className="text-gray-600">
                      {markdown.role === "user" ? "我" : "AI"}
                    </div>
                    <div
                      dangerouslySetInnerHTML={{ __html: markdown.content }}
                    ></div>
                  </div>
                );
              }
            })}
            {currentMessage && (
              <div dangerouslySetInnerHTML={{ __html: currentMarkdown }}></div>
            )}
          </div>
          <div className="mt-auto flex flex-row gap-2 self-stretch">
            <Input className="flex-grow" ref={inputRef}></Input>
            <Button
              size="icon"
              onClick={() => {
                const message = inputRef.current!.value;
                inputRef.current!.value = "";
                if (message.length === 0) {
                  return;
                }
                sendMessage(message);
              }}
            >
              <ArrowUp></ArrowUp>
            </Button>
          </div>
        </main>
      )}
    </>
  );
}
