
//fn todo_input(todo_list: &str){
//    println!("{}", todo_list );}
// press '+' for add todo list
// press '-' for remove todo list to finisch box
// press 'p' for add todo to the on process box
use std::io;
use std::collections::VecDeque;

fn todo_List() -> io::Result<()>{
    let mut todo_collection: VecDeque<String> = VecDeque::new();
    loop {

        let mut todo_input = String::new();
        io::stdin().read_line(&mut todo_input)?;
        
        let trimmed = todo_input.trim();
       
        //-------- Vec--------------------// 
        if trimmed == "exit"{
            break;
    
        }else if trimmed == "Done" {
            todo_collection.pop_front();
        }else if !trimmed.is_empty(){
            todo_collection.push_back(trimmed.to_string());
        };
        for (index,items) in todo_collection.iter().enumerate() {
            println!("{}. {}", index + 1, items);
        }
            
    

    }
    Ok(())  
        
}

//.push  -> add
//.pop -> remove

fn main() {
    println!("----- TO DO -----");
    println!("Please fill in the items you wish to do.");
    todo_List();
    
    if let Err(e) = todo_List(){
        println!("Error: {}", e);
    }
}
