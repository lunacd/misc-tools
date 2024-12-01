import { Metadata } from "next";
import NfoEditorClient from "./page.client";

export const metadata: Metadata = {
  title: "NFO Editor",
};

export default function NfoEditor() {
  return <NfoEditorClient />;
}
