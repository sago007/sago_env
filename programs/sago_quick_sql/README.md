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

## Connection Aliases (`sago:` prefix)

To avoid passing full connection strings (often containing credentials) on the command line or in environment variables, `sago_quick_sql` supports a connection alias mechanism via the `sago:` prefix.

If the connection string starts with `sago:`, the rest is treated as an alias name. The actual connection string is then read from the first line of:

```
<config_home>/sago_quick_sql/connections/<alias>.txt
```

`<config_home>` is platform-dependent:
- Linux: `~/.config` (or `$XDG_CONFIG_HOME` if set)

The alias name is also used to name the per-alias request log file under `<cache_dir>/sago_quick_sql/logs/<alias>.log`, which makes it convenient to track activity per database.

Example setup and invocation on Linux:

```bash
mkdir -p ~/.config/sago_quick_sql/connections
echo 'mysql:database=testschema;user=testuser;password=password' \
    > ~/.config/sago_quick_sql/connections/mydb.txt
chmod 600 ~/.config/sago_quick_sql/connections/mydb.txt

./build/sago_quick_sql --connect-string "sago:mydb" --sql "SELECT 1"
```

The alias form also works through the environment variable:

```bash
export SAGO_CONNECTION_STRING="sago:mydb"
./build/sago_quick_sql --sql "SELECT NOW()"
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
