import { Metadata } from "next";
import LoginClient from "./page.client";

export const metadata: Metadata = {
  title: "请登录"
}

export default function Login() {
  return <LoginClient></LoginClient>;
}
