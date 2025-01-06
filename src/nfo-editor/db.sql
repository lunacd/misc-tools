DROP TABLE IF EXISTS completions;
CREATE TABLE completions (
    completion_id INTEGER PRIMARY KEY,
    source TEXT NOT NULL,
    content TEXT NOT NULL
);

DROP INDEX IF EXISTS completions_index;
CREATE UNIQUE INDEX completions_index ON completions(source, content);
