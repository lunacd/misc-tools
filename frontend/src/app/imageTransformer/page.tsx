import { Metadata } from "next";
import ImageTransformerClient from "./page.client";

export const metadata: Metadata = {
  title: "Image Transformer",
};

export default function ImageTransformer() {
  return <ImageTransformerClient />;
}
