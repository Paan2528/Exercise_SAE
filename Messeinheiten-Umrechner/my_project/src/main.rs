use rusqlite::{Connection, Result};
use std::io;

fn main() -> Result<()> {
    loop {
        //+++++++++++++ CSV file ++++++++++++++++++
        let conn = Connection::open("app.db")?;

        conn.execute(
            "CREATE TABLE IF NOT EXISTS users(
            id INTEGER PRIMARY KEY,
            number REAL NOT NULL,
            choice TEXT NOT NULL,
            result REAL NOT NULL
            )",
            [],
        )?;
        //++++++++++++++++++++++++++++++++++++++++

        println!("Hello what do you want to do?");
        println!("1: millimeter -> Zentimeter");
        println!("2: Zentimeter -> Dezimeter");
        println!("3: Dezimeter -> Meter");
        println!("4: Beenden");

        let mut choice = String::new();
        io::stdin().read_line(&mut choice).unwrap();
        let choice = choice.trim();

        if choice == "--back" {
            break Ok(());
        }

        println!("Enter number:");

        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        let input = input.trim();
        let number: f64 = input.parse().unwrap();

        let result = match choice {
            "1" => number * 0.5,
            "2" => number * 0.5,
            "3" => number * 0.5,
            "4" => break Ok(()),

            _ => 0.0,
        };
        println!("{}", result);

        //++++++++++++++++++++++++
        conn.execute(
            "INSERT INTO users (number, choice, result) VALUES (?1, ?2, ?3)",
            (&number, &choice, &result),
        )?;

        println!("Saved to database!");
    }
}
