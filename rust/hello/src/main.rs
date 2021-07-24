use hex::FromHex;
use std::process;
use std::time::Instant;
use structopt::StructOpt;

mod aes_ctr_optimized;

/// Command line arguments struct
#[derive(StructOpt)]
#[structopt(
    name = "aes-ctr",
    about = "File encryption tool using AES in CTR mode."
)]
struct Cli {
    #[structopt(short = "c", long="command", required = true,
                help = concat!("Encryption or decryption command\n",
                               "(has to be 'encrypt' or 'decrypt')"))]
    command: String,
    #[structopt(short = "k", long = "key", required = true,
                help = concat!("Key for AES-CTR as hex string\n",
                               "(has to be of length 128-bit or 256-bit,\n",
                               "i.e. exactly 32 or 64 hex characters)"))]
    key: String,
    #[structopt(short = "v", long = "initialization-vector", required = true,
                help = concat!("Initialization Vector (IV) for AES-CTR as hex string\n",
                               "(has to be of length 128-bit,\n",
                               "i.e. exactly 32 hex characters)"))]
    iv: String,
    #[structopt(
        parse(from_os_str),
        short = "i",
        long = "input-file",
        required = true,
        help = "Path to input file"
    )]
    input_file_path: std::path::PathBuf,
    #[structopt(
        parse(from_os_str),
        short = "o",
        long = "output-file",
        required = true,
        help = "Path to output file"
    )]
    output_file_path: std::path::PathBuf,
}

/// Main function
fn main() {
    // Initialize some variables
    let mut errors = 0;
    let mut key_size: u16 = 0;
    let mut key_bytes: Vec<u8> = Vec::new();
    let mut iv_bytes: Vec<u8> = Vec::new();

    // Get command line arguments
    let args = Cli::from_args();

    // User message: checking and parsing command line arguments
    println!("\n### Checking and parsing command line arguments ...");

    // Check command
    if args.command != "encrypt" && args.command != "decrypt" {
        eprintln!(
            concat!(
                "!!! ERROR: Command not correct!\n",
                "!!!        (is '{}', but should be 'encrypt' or 'decrypt')"
            ),
            args.command
        );
        errors += 1;
    }

    // Check and parse key
    if args.key.len() == 32 {
        key_size = 128;
        match Vec::from_hex(args.key) {
            Ok(bytes) => key_bytes = bytes,
            Err(e) => {
                eprintln!("!!! ERROR: Key hex string parsing failed: {}", e);
                errors += 1;
            }
        };
    } else if args.key.len() == 64 {
        key_size = 256;
        match Vec::from_hex(args.key) {
            Ok(bytes) => key_bytes = bytes,
            Err(e) => {
                eprintln!("!!! ERROR: Key hex string parsing failed: {}", e);
                errors += 1;
            }
        };
    } else {
        eprintln!(
            concat!(
                "!!! ERROR: Key hex string neither matches 128-bit nor 256-bit size!\n",
                "!!!        (is {} characters long, but should be 32 or 64)"
            ),
            args.key.len()
        );
        errors += 1;
    }

    // Check and parse initializtion vector
    if args.iv.len() == 32 {
        match Vec::from_hex(args.iv) {
            Ok(bytes) => iv_bytes = bytes,
            Err(e) => {
                eprintln!(" ! ERROR: IV hex string parsing failed: {}", e);
                errors += 1;
            }
        };
    } else {
        eprintln!(
            concat!(
                "!!! ERROR: IV hex string does not match 128-bit size!\n",
                "!!!        (is {} characters long, but should be 32)"
            ),
            args.iv.len()
        );
        errors += 1;
    }

    // Handle encryption/decryption command with given parameters
    if errors == 0 {
        println!("\n### Performing {}ion ...", args.command);
        let now = Instant::now();
        aes_ctr_optimized::handle_aes_ctr_command(
            args.command,
            key_size,
            key_bytes,
            iv_bytes,
            args.input_file_path,
            args.output_file_path,
        );
        println!(
            "\n### Finished! It took {:.10} seconds!",
            now.elapsed().as_secs_f32()
        );
    } else {
        println!("\n### Aborting ...");
        println!("!!! {} error(s) occurred!", errors);
        process::exit(1);
    }
}
