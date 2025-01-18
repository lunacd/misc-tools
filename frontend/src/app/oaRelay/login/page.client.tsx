"use client";

import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { useRouter } from "next/navigation";
import { useRef } from "react";

export default function LoginClient() {
  const usernameRef = useRef<HTMLInputElement>(null);
  const passwordRef = useRef<HTMLInputElement>(null);
  const router = useRouter();
  return (
    <div className="flex flex-col items-center p-8">
      <div className="max-w-96 space-y-4">
        <Input placeholder="用户名" ref={usernameRef} />
        <Input placeholder="密码" ref={passwordRef} />
        <Button
          className="w-full"
          onClick={async () => {
            const res = await fetch("/oaRelay/signIn", {
              method: "POST",
              body: JSON.stringify({
                username: usernameRef.current!.value,
                password: passwordRef.current!.value,
              }),
              credentials: "include",
            });
            if (res.ok) {
              router.push("/oaRelay");
            }
          }}
        >
          登录
        </Button>
      </div>
    </div>
  );
}
