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
