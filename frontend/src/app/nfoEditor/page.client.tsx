"use client";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";

import {
  SuggestionInput,
  SuggestionInputHandle,
} from "@/components/SuggestionInput";
import { Button } from "@/components/ui/button";
import { useRef } from "react";
import { ListInput, ListInputHandle } from "@/components/ListInput";

export default function NfoEditorClient() {
  const titleRef = useRef<HTMLInputElement>(null);
  const studioRef = useRef<SuggestionInputHandle>(null);
  const actorRef = useRef<ListInputHandle>(null);
  const tagRef = useRef<ListInputHandle>(null);
  const fileRef = useRef<HTMLInputElement>(null);

  return (
    <div className="m-8 flex w-96 flex-col gap-4">
      <div className="gap-1.5">
        <Label>Title</Label>
        <Input ref={titleRef} />
      </div>
      <div className="gap-1.5">
        <Label>Studio</Label>
        <SuggestionInput completionSource="studio" ref={studioRef} />
      </div>

      <div className="gap-1.5">
        <Label>Actors</Label>
        <ListInput className="mt-2" completionSource="actors" ref={actorRef} />
      </div>
      <div className="gap-1.5">
        <Label>Tags</Label>
        <ListInput className="mt-2" completionSource="tags" ref={tagRef} />
      </div>

      <div className="gap-1.5">
        <Label>Media File</Label>
        <Input type="file" ref={fileRef} />
      </div>

      <Button
        className="mt-2"
        onClick={() => {
          const fileSegments = fileRef.current!.value.split("\\");
          const filename = fileSegments[fileSegments.length - 1];
          fetch("http://localhost:8000/nfoEditor/saveToNfo", {
            method: "POST",
            body: JSON.stringify({
              title: titleRef.current!.value,
              studio: studioRef.current!.getValue(),
              tags: tagRef.current!.getValues(),
              actors: actorRef.current!.getValues(),
              filename: filename,
            }),
          }).then(() => {
            titleRef.current!.value = "";
            studioRef.current!.clear();
            actorRef.current!.clear();
            tagRef.current!.clear();
            fileRef.current!.value = "";
          });
        }}
      >
        Create
      </Button>
    </div>
  );
}
