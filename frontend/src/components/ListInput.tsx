"use client";

import { forwardRef, useImperativeHandle, useRef, useState } from "react";
import { Badge } from "./ui/badge";
import { X, Plus } from "lucide-react";
import { SuggestionInput, SuggestionInputHandle } from "./SuggestionInput";
import { Button } from "./ui/button";

export interface ListInputHandle {
  clear: () => void;
  getValues: () => string[];
}

interface ListInputProps {
  className?: string;
  onChange?: (value: string[]) => void;
  completionSource: string;
}

export const ListInput = forwardRef<
  ListInputHandle,
  ListInputProps
>(function (props, ref) {
  const [values, setValues] = useState<string[]>([]);
  const inputRef = useRef<SuggestionInputHandle>(null);
  function acceptInput() {
    setValues((orig) => {
      return [...orig, inputRef.current!.getValue()];
    });
    if (props.onChange) {
      props.onChange(values);
    }
    inputRef.current!.clear();
  }

  useImperativeHandle(ref, () => {
    return {
      clear: () => setValues([]),
      getValues: () => values,
    };
  });

  return (
    <div className="flex flex-col gap-1.5">
      <div className="flex w-full flex-row gap-2">
        <SuggestionInput
          completionSource={props.completionSource}
          onKeydown={(e) => {
            if (e.key === "Enter") {
              acceptInput();
            }
          }}
          className="flex-grow"
          ref={inputRef}
        />
        <Button variant="outline" onClick={acceptInput} tabIndex={-1}>
          <Plus size={16} />
        </Button>
      </div>
      {values.length > 0 && (
        <div className="flex flex-row flex-wrap gap-1.5">
          {values.map((value, index) => (
            <Badge
              variant="outline"
              className="flex flex-row gap-1"
              key={index}
            >
              {value}{" "}
              <span
                className="cursor-pointer rounded-full p-0.5 hover:bg-accent"
                onClick={() => {
                  setValues((orig) => {
                    const arr = [...orig];
                    arr.splice(index, 1);
                    return arr;
                  });
                }}
              >
                <X size={16} />
              </span>
            </Badge>
          ))}
        </div>
      )}
    </div>
  );
});
ListInput.displayName = "ListInput";
