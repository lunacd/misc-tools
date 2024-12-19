"use client";

import { ArrowUp } from "lucide-react";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import {
  Sidebar,
  SidebarContent,
  SidebarGroup,
  SidebarGroupContent,
  SidebarGroupLabel,
  SidebarMenu,
  SidebarMenuButton,
  SidebarMenuItem,
  SidebarProvider,
} from "@/components/ui/sidebar";
import Link from "next/link";
import { useEffect, useRef, useState } from "react";
import { SSE } from "sse.js";
import DOMPurify from "isomorphic-dompurify";
import { marked } from "marked";
import styles from "./page.module.css";

interface Message {
  role: string;
  content: string;
}

type CurrentMessage = Message & { finished: boolean };

export default function OaRelayClient() {
  const [messages, setMessages] = useState<Message[]>([
    { role: "system", content: "You are a helpful assistant" },
  ]);
  const [lastProcessedMessage, setLastProcessedMessage] = useState<number>(0);
  const inputRef = useRef<HTMLInputElement>(null);
  const [currentMessage, setCurrentMessage] = useState<
    CurrentMessage | undefined
  >(undefined);

  useEffect(() => {
    if (
      messages[messages.length - 1].role !== "user" ||
      lastProcessedMessage >= messages.length
    ) {
      return;
    }

    // User has sent a message, get completion
    setLastProcessedMessage(messages.length);
    const source = new SSE("/oaRelay/completions", {
      headers: {
        "Content-Type": "application/json",
      },
      payload: JSON.stringify({
        messages: messages,
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
      console.log(data);
    });
  }, [messages, lastProcessedMessage]);

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
    <SidebarProvider>
      <Sidebar>
        <SidebarContent>
          <SidebarGroup>
            <SidebarGroupLabel>过往对话</SidebarGroupLabel>
            <SidebarGroupContent>
              <SidebarMenu>
                <SidebarMenuItem>
                  <SidebarMenuButton asChild>
                    <Link href="/">abc</Link>
                  </SidebarMenuButton>
                </SidebarMenuItem>
              </SidebarMenu>
            </SidebarGroupContent>
          </SidebarGroup>
        </SidebarContent>
      </Sidebar>
      <main className="flex flex-grow flex-col space-y-4 p-4">
        <div className="flex-grow overflow-y-scroll space-y-4">
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
              setMessages((orig) => [
                ...orig,
                { role: "user", content: message },
              ]);
            }}
          >
            <ArrowUp></ArrowUp>
          </Button>
        </div>
      </main>
    </SidebarProvider>
  );
}
