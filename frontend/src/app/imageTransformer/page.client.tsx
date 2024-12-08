"use client";

import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { useRef, useState } from "react";

export default function ImageTransformerClient() {
  const imageWidthRef = useRef<HTMLInputElement>(null);
  const fileRef = useRef<HTMLInputElement>(null);
  const [downloadUrl, setDownloadUrl] = useState<string | undefined>(undefined);
  const [totalFiles, setTotalFiles] = useState<number>(0);
  const [currentFile, setCurrentFile] = useState<number>(1);

  function timeout(ms: number) {
    return new Promise((resolve) => setTimeout(resolve, ms));
  }

  async function convertImage() {
    if (
      imageWidthRef.current!.value === "" ||
      !fileRef.current!.files ||
      fileRef.current!.files.length == 0
    ) {
      return;
    }
    const imageWidth = parseInt(imageWidthRef.current!.value);
    setTotalFiles(fileRef.current!.files.length);
    let index = 1;
    setCurrentFile(index);
    for (const file of fileRef.current!.files) {
      const res = await fetch(
        `/imageTransformer/convert?targetWidth=${encodeURIComponent(imageWidth)}&filename=${encodeURIComponent(file.name)}`,
        {
          method: "POST",
          body: file,
        },
      );
      const uuid = (await res.json()).uuid;
      while (true) {
        await timeout(500);
        const res = await fetch(
          `/imageTransformer/status?uuid=${encodeURIComponent(uuid)}`,
        );
        if (res.status == 200) {
          // Conversion ready
          setDownloadUrl(
            `/imageTransformer/download?uuid=${encodeURIComponent(uuid)}`,
          );
          break;
        }
      }
      index += 1;
      setCurrentFile(index);
    }
  }
  return (
    <div className="m-8 flex w-96 flex-col gap-4">
      <div className="space-y-1.5">
        <Label>Image Width</Label>
        <Input ref={imageWidthRef} />
      </div>
      <div className="space-y-1.5">
        <Label>Images</Label>
        <Input type="file" multiple ref={fileRef} />
      </div>
      <Button onClick={convertImage}>Convert</Button>
      <iframe className="hidden" src={downloadUrl} />
      {currentFile <= totalFiles && (
        <div>
          Converting {currentFile} of {totalFiles}...
        </div>
      )}
    </div>
  );
}
