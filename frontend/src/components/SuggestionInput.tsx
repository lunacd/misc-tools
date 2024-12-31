"use client";
import {
  ChangeEventHandler,
  forwardRef,
  KeyboardEventHandler,
  useCallback,
  useImperativeHandle,
  useRef,
  useState,
} from "react";
import { Input } from "./ui/input";
import classNames from "classnames";

export interface SuggestionInputHandle {
  clear: () => void;
  getValue: () => string;
}

interface SuggestionInputProps {
  completionSource: string;
  onChange?: ChangeEventHandler<HTMLInputElement>;
  className?: string;
  onKeydown?: KeyboardEventHandler<HTMLInputElement>;
}

export const SuggestionInput = forwardRef<
  SuggestionInputHandle,
  SuggestionInputProps
>((props: SuggestionInputProps, ref) => {
  const [completions, setCompletions] = useState<string[]>([]);
  const inputRef = useRef<HTMLInputElement>(null);
  const [selected, setSelected] = useState(0);

  const autocomplete = useCallback(
    function (str: string) {
      if (str.length >= 2) {
        fetch(
          `/nfoEditor/complete?source=${encodeURIComponent(props.completionSource)}&str=${encodeURIComponent(str)}`,
        )
          .then((res) => res.json())
          .then((json) => {
            setCompletions(json.completions);
          });
      } else {
        setCompletions([]);
      }
    },
    [setCompletions, props.completionSource],
  );

  useImperativeHandle(ref, () => {
    return {
      clear() {
        inputRef.current!.value = "";
        autocomplete("");
      },
      getValue() {
        return inputRef.current!.value;
      },
    };
  });

  const acceptSuggestion = useCallback(
    (selectedIndex: number) => {
      if (selectedIndex >= 0 && selectedIndex < completions.length) {
        inputRef.current!.value = completions[selectedIndex];
        setCompletions([]);
        setSelected(0);
      }
    },
    [inputRef, completions],
  );

  return (
    <div className={`${props.className ?? ""}`}>
      <Input
        onChange={(e) => {
          autocomplete(e.target.value);
          if (props.onChange) {
            props.onChange(e);
          }
        }}
        onKeyDown={(e) => {
          if (e.key === "ArrowUp") {
            setSelected((orig) => {
              return Math.max(orig - 1, 0);
            });
            e.preventDefault();
          } else if (e.key === "ArrowDown") {
            setSelected((orig) => {
              return Math.min(orig + 1, completions.length - 1);
            });
            e.preventDefault();
          } else if (e.key === "Enter") {
            acceptSuggestion(selected);
          }
          if (props.onKeydown) {
            props.onKeydown(e);
          }
        }}
        ref={inputRef}
      />
      {completions.length > 0 && (
        <div className="relative h-0">
          <div className="absolute top-2 w-full rounded-md border bg-white p-1 shadow-md">
            {completions.map((completion, index) => (
              <div
                className={classNames(
                  "cursor-pointer select-none rounded-sm px-2 py-1.5 hover:bg-accent focus:bg-accent",
                  {
                    "bg-accent": index === selected,
                  },
                )}
                key={index}
                onClick={() => {
                  acceptSuggestion(index);
                }}
              >
                {completion}
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  );
});
SuggestionInput.displayName = "SuggestionInput";
