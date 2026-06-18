use rand::Rng;
use std::io;

fn guss() {
    let my_num = rand::thread_rng().gen_range(1..=100);

    loop {
        let mut input = String::new();

        println!("Guss the Number is...?");

        io::stdin()
            .read_line(&mut input)
            .expect("Failed to read line");
        let guss_num: i32 = input.trim().parse().expect("Please enter a valid number");

        if guss_num < my_num {
            println!("It's more then {}", guss_num);
        } else if guss_num > my_num {
            println!("It's less then {}", guss_num);
        } else if guss_num == my_num {
            println!("{} is correct!", guss_num);
            break;
        } else {
            println!("Pleasse put the number only.")
        }
    }
}

fn main() {
    guss();
}
