#![allow(unused)]

#[doc(inline)]
use hex::{ToHex, FromHex};
use std::{
    io::{self, BufReader, Read, Write, SeekFrom, Seek, prelude::*},
    time::{self, Duration, Instant},
    fs::{File, OpenOptions},
    path::PathBuf,
    convert::TryInto,
    str,
};

fn main() -> io::Result<()> {
    let start: Instant = Instant::now();
    // Files
    let mut output_filename: &str = "data/output.txt";
    let mut input_filename: &str = "data/input.tmp";
    let mut key_filename: &str = "data/key.txt";
    let mut iv_filename: &str = "data/iv.txt";

    let mut input_reader: BufReader<File> = BufReader::new(File::open(input_filename)?);
    let mut key_reader: BufReader<File> = BufReader::new(File::open(key_filename)?);
    let mut iv_reader: BufReader<File> = BufReader::new(File::open(iv_filename)?);
    let mut output_file: File = File::create(output_filename)?;

    // Einige Variablen initialisieren
    let mut key_size: u16 = 128;
    let mut sbox: Vec<u8> = vec![0; 256];
    let mut ibox: Vec<u8> = vec![0; 256];
    let mut key_bytes: Vec<u8> = vec![0; 32 as usize];
    let mut iv_bytes: Vec<u8> = vec![0; 32 as usize];
    let length: u64 = input_reader.seek(SeekFrom::End(0))?; //Datei Länge
    let mut blocks = length / 16 / 2;
    println!("Länge der Inputdaten:{}", length);
    println!("Anzahl von 16-Bytes-Blöcken:{}", blocks);

    key_reader.read(&mut key_bytes)?;
    iv_reader.read(&mut iv_bytes)?;

    let mut key_bytes: Vec<u8> = Vec::from_hex(&key_bytes).unwrap();
    let mut iv_bytes: Vec<u8> = Vec::from_hex(&iv_bytes).unwrap();
    let (nk, nr) = match key_size {
        128 => (4 as usize, 10 as usize),
        _ => (8 as usize, 14 as usize),
    };

    initialize_aes_sbox(&mut sbox, &mut ibox); // Initialisierung von sbox und Ibox
    let keys: Vec<u8> = key_expansion(&key_bytes, nk, nr, &sbox);
    let mut iv_blocks: Vec<Vec<u8>> = Vec::new();
    iv_blocks.push(iv_bytes.clone());
    for i in 1..blocks {
        iv_blocks.push(ctr128_inc(&iv_blocks[i as usize - 1], 1));
    }
    let mut iv_blocks: Vec<u8> = iv_blocks.concat();
    let mut test: Vec<u8> = (0..length).map(|_| 0).collect();
    let mut test_file: File = File::open("data/input.tmp")?;
    let mut test_reader: BufReader<File> = BufReader::new(test_file);
    let length = test_reader.read(&mut test)?;
    let mut test: Vec<&[u8]> = test.chunks(32).collect();
    if test[test.len() - 1] == [13, 10] {
        test.pop();
    }
    let mut test: String = test
        .iter()
        .enumerate()
        .map(|(i, &data)| aes(&mut iv_blocks[i * 16..(i + 1) * 16].to_vec(), data, &keys, nr, &sbox, 0))
        .collect::<String>();

    println!("EXECUTION TIME: {:?}", start.elapsed());
    let start = Instant::now();
    output_file.write(test.as_ref());
    println!("EXECUTION TIME: {:?}", start.elapsed());
    Ok(())
}

// Function to handle encryption/decryption command with given parameters
pub fn handle_aes_ctr_command(
    command: String,
    key_size: u16,
    key_bytes: Vec<u8>,
    iv_bytes: Vec<u8>,
    input_file_path: PathBuf,
    output_file_path: PathBuf,
) {
    println!("\n### Dummy printing ...");
    println!(" - command           = {}", command);
    println!(" - key_size          = {}", key_size);
    println_bytes(" - key_bytes         = ", &key_bytes);
    println_bytes(" - iv_bytes          = ", &iv_bytes);
    println!(" - input_file_path   = {}", input_file_path.display());
    println!(" - output_file_path  = {}", output_file_path.display());

    /// Globale Informationen
    const NR_T: u64 = 64; /// Anzahl von Thread.
    const OFFSET: u64 = 16 * NR_T;
    let mut f = File::open(&input_file_path).ok().unwrap();
    let length = f.seek(SeekFrom::End(0)).ok().unwrap(); /// Datei Laenge
    let mut nbs: u64 = length / OFFSET; /// Anzahl der sequentielle Ausführung
    let mut rest: usize = (length % OFFSET) as usize;
    let mut blocks = length / 16 / 2;
    let mut blocks = length / 16 ;
    let mut sbox: Vec<u8> = vec![0; 256];
    let mut ibox: Vec<u8> = vec![0; 256];
    let (nk, nr) = match key_size {
        128 => (4 as usize, 10 as usize),
        _ => (8 as usize, 14 as usize),
    };
    initialize_aes_sbox(&mut sbox, &mut ibox); /// Initialisierung von sbox und Ibox
    let keys: Vec<u8> = key_expansion(&key_bytes, nk, nr, &sbox);
    let mut iv_blocks: Vec<Vec<u8>> = Vec::new();
    iv_blocks.push(iv_bytes.clone());
    for i in 1..blocks {
        iv_blocks.push(ctr128_inc(&iv_blocks[i as usize - 1], 1));
    }
    let mut iv_blocks: Vec<u8> = iv_blocks.concat();
    let mut test_reader: BufReader<File> = BufReader::new(File::open(input_file_path).ok().unwrap());
    let mut test: Vec<u8> = (0..length).map(|_| 0).collect();
    let length = test_reader.read(&mut test);
    // let mut test: Vec<&[u8]> = test.chunks(32).collect();
    let mut test: Vec<&[u8]> = test.chunks(16).collect();
    if test[test.len() - 1] == [13, 10] {
        test.pop();
    }
    println!("Tesst le={}", test.len());
    let mut test: String = test
        .iter()
        .enumerate()
        .map(|(i, &data)| aes(&mut iv_blocks[i * 16..(i + 1) * 16].to_vec(), data, &keys, nr, &sbox, 0))
        .collect::<String>();
    // if !std::path::Path::new(&output_file_path).exists() {
    //     File::create(&output_file_path).ok().unwrap();
    // }
    File::create(&output_file_path).ok().unwrap();
    let mut f = OpenOptions::new()
        .write(true)
        .append(true)
        .open(&output_file_path)
        .unwrap();
    f.write(test.as_ref());
    println!("{}", test);
}

/// Function to print bytes
fn println_bytes(name_str: &str, bytes: &Vec<u8>) {
    print!("{}", name_str);
    println!("{:02x?}", bytes);
}

/// Funktion für die Initialisierung von sbox und Ibox
fn initialize_aes_sbox(sbox: &mut Vec<u8>, ibox: &mut Vec<u8>) {
    /// `x` um `shift` verschieben
    fn link_shift(x: u8, shift: u8) -> u8 {
        (x << shift) | (x >> (8 - shift))
    }
    let mut p: u8 = 1;
    let mut q: u8 = 1;
    loop {
        p = p ^ (p << 1) ^ if p & 0x80 != 0 { 0x1B } else { 0 };
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= if q & 128 != 0 { 0x9 } else { 0 };
        let xformed: u8 =
            q ^ link_shift(q, 1) ^ link_shift(q, 2) ^ link_shift(q, 3) ^ link_shift(q, 4);
        sbox[p as usize] = xformed ^ 0x63;
        ibox[sbox[p as usize] as usize] = p;
        if p == 1 {
            break;
        }
    }
    sbox[0] = 0x63;
}

/// Durchführung der XOR Operation zwischen `v1` und `v2`.
fn add_round_keys(v1: &Vec<u8>, v2: &[u8], print_: bool) -> Vec<u8> {
    if print_ {
        println!("v1= {:02x?}", v1);
        println!("v2= {:02x?}", v2);
    }
    v1.iter().zip(v2.iter()).map(|(&x1, &x2)| x1 ^ x2).collect()
}


/// Substitution von einer Vector
fn sub_bytes(row: &mut Vec<u8>, sbox: &Vec<u8>) {
    row.iter_mut().for_each(|x: &mut u8| *x = sbox[*x as usize])
}

/// Rotiere die Einträge von `vector` um `shift`
fn shift_rows(state: &mut Vec<u8>) {
    *state = vec![0, 1, 2, 3, 5, 6, 7, 4, 10, 11, 8, 9, 15, 12, 13, 14]
        .into_iter()
        .map(|i| state[i])
        .collect();
}

/// Spalten mischen
fn mix_columns(state: &mut Vec<u8>) {
    ///  Multiplikation von zwei Zahlen in GF(2^8) Raum
    fn gmul(p: u8, q: u8) -> u8 {
        let mut a: u8 = p;
        let mut b: u8 = q;
        let mut r: u8 = 0;
        for _ in 0..8 {
            if b & 0x01 != 0 {
                r ^= a;
            }
            if a & 0x80 != 0 {
                a <<= 1;
                a ^= 0x1b;
            } else {
                a <<= 1;
            }
            b >>= 1;
        }
        r
    }

    //Matrixmultiplikation wie in den originalen Artikel
    let copy: Vec<u8> = state.clone();
    for i in 0..4 {
        state[i] = gmul(copy[i], 0x2) ^ gmul(copy[i + 4], 0x3) ^ copy[i + 8] ^ copy[i + 12];
        state[i + 4] = copy[i] ^ gmul(copy[i + 4], 0x2) ^ gmul(copy[i + 8], 0x3) ^ copy[i + 12];
        state[i + 8] = copy[i] ^ copy[i + 4] ^ gmul(copy[i + 8], 0x2) ^ gmul(copy[i + 12], 0x3);
        state[i + 12] = gmul(copy[i], 0x3) ^ copy[i + 4] ^ copy[i + 8] ^ gmul(copy[i + 12], 0x2);
    }
}

/// Der Zähler um c inkrementieren
fn ctr128_inc(counter: &Vec<u8>, c: u64) -> Vec<u8> {
    let mut c = c;
    let mut n: usize = 16;
    let mut count: Vec<u8> = counter.clone();

    loop {
        n -= 1;
        c += count[n] as u64;
        count[n] = c as u8;
        c >>= 8;
        if n == 0 {
            break count;
        }
    }
}

/// Funktion fuer die Schlüsselerweiterung
fn key_expansion(key: &Vec<u8>, nk: usize, nr: usize, sbox: &Vec<u8>) -> Vec<u8> {
    let mut index: usize = key.len();
    let mut pred: usize = index - 4;
    let mut pred_: usize = 0;
    const NB: usize = 4;
    const RCON: [u8; 15] = [1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d,
        0x9a];
    let mut w: Vec<u8> = vec![0; 4 * NB * (nr + 1)]; // Der originale Schlüssel kopieren.
    for (i, k) in key.iter().enumerate() {
        w[i] = *k;
    }

    for i in nk..(NB * (nr + 1)) {
        if i % nk == 0 {
            // temp = SubWord(RotWord(temp)) xor Rcon[i/Nk]
            // w[i] = w[i-1] xor w[i-Nk]
            w[index + 0] = sbox[w[pred + 1] as usize] ^ RCON[(i / nk) - 1] ^ w[pred_ + 0];
            w[index + 1] = sbox[w[pred + 2] as usize] ^ w[pred_ + 1];
            w[index + 2] = sbox[w[pred + 3] as usize] ^ w[pred_ + 2];
            w[index + 3] = sbox[w[pred + 0] as usize] ^ w[pred_ + 3];
        } else if (nk > 6) & (i % nk == 4) {
            // temp = SubWord(temp)
            // w[i] = w[i-1] xor w[i-Nk]
            w[index + 0] = sbox[w[pred + 0] as usize] ^ w[pred_ + 0];
            w[index + 1] = sbox[w[pred + 1] as usize] ^ w[pred_ + 1];
            w[index + 2] = sbox[w[pred + 2] as usize] ^ w[pred_ + 2];
            w[index + 3] = sbox[w[pred + 3] as usize] ^ w[pred_ + 3];
        } else {
            // w[i] = w[i-1] xor w[i-Nk]
            w[index + 0] = w[pred + 0] ^ w[pred_ + 0];
            w[index + 1] = w[pred + 1] ^ w[pred_ + 1];
            w[index + 2] = w[pred + 2] ^ w[pred_ + 2];
            w[index + 3] = w[pred + 3] ^ w[pred_ + 3];
        }
        pred += 4;
        pred_ += 4;
        index += 4;
    }
    for i in (0..w.len()).step_by(16) {
        let temp: Vec<u8> = rot(&w[i..i + 16]);
        for j in 0..16 {
            w[i + j] = temp[j];
        }
    }
    w
}

fn rot(state: &[u8]) -> Vec<u8> {
    let indexes: Vec<usize> = vec![0usize, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15];
    indexes.iter()
        .map(|&i| state[i])
        .collect()
}

fn aes(mut state: &mut Vec<u8>, data: &[u8], keys: &Vec<u8>, nr: usize, sbox: &Vec<u8>, count: u64) -> String {
    let indexes: Vec<usize> = vec![0usize, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15];
    // let data: Vec<u8> = Vec::from_hex(data).unwrap();
    *state = ctr128_inc(&state, count);
    *state = indexes.iter()
        .map(|&i| state[i])
        .collect();
    *state = add_round_keys(&state, &keys[0..16], false);
    for i in 1..nr {
        sub_bytes(&mut state, &sbox);
        shift_rows(&mut state);
        mix_columns(&mut state);
        *state = add_round_keys(&mut state, &keys[i * 16..(i + 1) * 16], false);
        if i == nr - 1 {
            sub_bytes(&mut state, &sbox);
            shift_rows(&mut state);
            *state = add_round_keys(&state, &keys[(i + 1) * 16..(i + 2) * 16], false);
        }
    }

    // *state = indexes.iter()
    //         .map(|&i| state[i])
    //         .collect();
    indexes.iter()
        .enumerate()
        .map(|(i, &index)| format!("{:02x}", state[index] ^ data[i]))
        .collect::<String>()
}