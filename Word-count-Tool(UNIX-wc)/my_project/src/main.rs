use std::fs;

fn main() {
    let path = "text.txt";

    let content = fs::read_to_string(path).expect("Datei konnte nicht geladen werden");

    let world_count = content.split_ascii_whitespace().count();

    println!("Anzahl der Wörter: {}", world_count);
}
