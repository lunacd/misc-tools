import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { useRouter } from "next/navigation";
import { useRef } from "react";

export default function LoginClient() {
  const usernameRef = useRef<HTMLInputElement>(null);
  const passwordRef = useRef<HTMLInputElement>(null);
  const router = useRouter();
  return (
    <div>
      <Input placeholder="用户名" ref={usernameRef} />
      <Input placeholder="密码" ref={passwordRef} />
      <Button
        onClick={async () => {
          const res = await fetch("/oaRelay", {
            method: "POST",
            body: JSON.stringify({
              username: usernameRef.current!.value,
              password: passwordRef.current!.value,
            }),
          });
          if (res.ok) {
            router.push("/");
          }
        }}
      >
        登录
      </Button>
    </div>
  );
}
