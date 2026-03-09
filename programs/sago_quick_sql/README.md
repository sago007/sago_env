# sago_quick_sql

`sago_quick_sql` is a small C++ command-line utility for running SQL quickly from scripts, terminals, and automation.

It supports:
- `SELECT` queries with row output
- `INSERT`/`UPDATE`/`DELETE`/DDL execution via `--exec`
- Positional bind arguments via repeated `--bind`
- CSV-safe output via `--csv`
- Basic result-count checks via `--min`

The tool uses:
- `boost::program_options` for CLI parsing
- `cppdb` for database access

## Build

Requirements:
- CMake
- C++ compiler with C++11 support
- Boost (`program_options`)
- `cppdb` development libraries

Build commands:

```bash
cmake -S . -B build
cmake --build build
```

Binary output:
- `build/sago_quick_sql`

## Quick Usage

Show help:

```bash
./build/sago_quick_sql --help
```

Run a `SELECT`:

```bash
./build/sago_quick_sql \
  --connect-string "mysql:database=testschema;user=testuser;password=password" \
  --sql "SELECT id, name FROM users WHERE id = ?" \
  --bind "42"
```

Run an `UPDATE`/`INSERT`/`DELETE` and print affected row count:

```bash
./build/sago_quick_sql \
  --connect-string "mysql:database=testschema;user=testuser;password=password" \
  --sql "UPDATE users SET active = ? WHERE id = ?" \
  --bind "1" \
  --bind "42" \
  --exec
```

Use environment variable instead of `--connect-string`:

```bash
export SAGO_CONNECTION_STRING="odbc:DSN=mydb"
./build/sago_quick_sql --sql "SELECT NOW()"
```

CSV output:

```bash
./build/sago_quick_sql --sql "SELECT id, comment FROM logs" --csv
```

## Command-Line Options

- `--help`, `-h`: Print usage
- `--version`: Print version
- `--connect-string <value>`: DB connection string
- `--sql <value>`: SQL statement to run
- `--bind <value>`: Bind parameter (repeat for multiple parameters)
- `--exec`: Execute non-SELECT and commit
- `--silent`, `-s`: Do not print query rows
- `--min <int>`: Return non-zero if SELECT returns fewer rows than this
- `--max <int>`: Parsed, but currently not enforced in runtime logic
- `--csv`: CSV-escape and quote output values when needed
- `--strip-control`: Remove control chars from output values
- `--printargs`: Print effective args and exit

## Behavior Notes

- `SELECT` flow starts a transaction and rolls it back after reading results.
- `--exec` flow commits transaction after execution.
- If `--sql` is missing, the program exits non-zero.

## Letting an AI Agent Access the Database Using Skills

A safe pattern is to give the AI agent a **skill** that only allows DB access through this binary. That keeps connection handling and SQL execution in one controlled path.

Typical approach:

1. Create a skill folder (for example `.github/copilot/skills/sql-runner/`).
2. Add a `SKILL.md` describing when to use the skill.
3. In the skill instructions, require the agent to call `sago_quick_sql` with:
   - SQL passed in `--sql`
   - bind values passed with repeated `--bind`
   - connection string from `SAGO_CONNECTION_STRING` (not hardcoded)
4. Restrict behavior in the skill:
   - default to read-only queries unless explicitly asked to modify data
   - require `--exec` for write operations
   - forbid dangerous statements (`DROP`, `TRUNCATE`, etc.) unless user confirms
   - require `--min` checks for validation workflows

Example skill instruction snippet:

```md
# Skill: SQL Runner via sago_quick_sql

Use this skill when the user asks for database reads/writes.

Rules:
- Use `/home/poul/git/sago_env/programs/sago_quick_sql/build/sago_quick_sql`.
- Read connection from `SAGO_CONNECTION_STRING`.
- Pass SQL only through `--sql`.
- Pass values through repeated `--bind` parameters.
- For write SQL, require explicit user confirmation and add `--exec`.
- Prefer `SELECT` first to verify target rows before modifying data.
```

Example command pattern an AI agent can execute:

```bash
export SAGO_CONNECTION_STRING="odbc:DSN=mydb"
/home/poul/git/sago_env/programs/sago_quick_sql/build/sago_quick_sql \
  --sql "SELECT id, email FROM users WHERE status = ?" \
  --bind "active" \
  --csv
```

This setup gives the agent practical DB access while preserving an auditable, constrained execution path.
