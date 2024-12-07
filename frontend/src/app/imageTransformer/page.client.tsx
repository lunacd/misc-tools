"use client";

import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";

export default function ImageTransformerClient() {
  return (
    <div className="m-8 flex w-96 flex-col gap-4">
      <div className="space-y-1.5">
        <Label>Image Width</Label>
        <Input />
      </div>
      <div className="space-y-1.5">
        <Label>Images</Label>
        <Input type="file" multiple />
      </div>
      <Button>Convert</Button>
    </div>
  );
}
