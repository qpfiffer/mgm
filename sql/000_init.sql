CREATE TABLE entry (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at_date TEXT NOT NULL,
    created_at_time TEXT NOT NULL,
    parent_id INTEGER NULL,
    data TEXT NOT NULL,
    FOREIGN KEY (parent_id) REFERENCES entry(id)
);
