import { Metadata } from "next";
import OaRelayClient from "./page.client";

export const metadata: Metadata = {
  title: "OA Relay",
};

export default function OaRelay() {
  return <OaRelayClient></OaRelayClient>;
}
