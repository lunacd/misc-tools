DROP TABLE IF EXISTS users;
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    username TEXT NOT NULL,
    password TEXT NOT NULL
);

DROP INDEX IF EXISTS username_index;
CREATE UNIQUE INDEX username_index ON users(username);

DROP TABLE IF EXISTS sessions;
CREATE TABLE sessions (
    session_row_id SERIAL PRIMARY KEY,
    session_id INTEGER NOT NULL,
    user_id INTEGER NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

DROP INDEX IF EXISTS session_id_index;
CREATE UNIQUE INDEX session_id_index ON sessions(session_id);

DROP TABLE IF EXISTS messages;
CREATE TABLE messages (
    message_id SERIAL PRIMARY KEY,
    session_row_id INTEGER NOT NULL,
    role TEXT NOT NULL,
    message TEXT NOT NULL,
    msg_order INTEGER NOT NULL,
    FOREIGN KEY (session_row_id) REFERENCES sessions(session_row_id)
);

DROP INDEX IF EXISTS message_order_index;
CREATE UNIQUE INDEX message_order_index ON messages(msg_order);
