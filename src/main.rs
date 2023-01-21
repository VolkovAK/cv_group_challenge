use std::time::Instant;
use rand::{self, Rng};

struct SeriesResult {
    start: usize,
    stop: usize,
    len: usize,
}

fn find_longest_series(series: &Vec<i32>) -> SeriesResult {
    let mut edges_current = [0, 0];
    let mut edges_biggest = [0, 0];
    let mut current_direction = 0;
    let mut prev_direction = 0;
    
    for i in 0..series.len() {
        if edges_current[1] - edges_current[0] > edges_biggest[1] - edges_biggest[0] {
            edges_biggest[0] = edges_current[0];
            edges_biggest[1] = edges_current[1];
        }
        if i == series.len() - 1 {
            break;
        }
        if series[i+1] - series[i] > 0 {
            current_direction = 1;
        }
        else if series[i+1] - series[i] < 0 {
            current_direction = -1;
        }
        // no diff, still monotonic
        edges_current[1] = i+1;

        if prev_direction != current_direction && prev_direction != 0{
            edges_current[0] = i;
        }
        prev_direction = current_direction;

    }
    let len = if series.len() == 0 { 0 } else { edges_biggest[1] - edges_biggest[0] + 1};
    let res = SeriesResult { start: edges_biggest[0], stop: edges_biggest[1], len };

    res
}
            
fn benchmark() {
    let mut rng = rand::thread_rng();

    let mut test: Vec<i32> = (0..100).map(|_| rng.gen_range(-100..100)).collect(); 
    let before = Instant::now();
    for _ in 0..10000 {
        test[0] = rng.gen_range(-100..100);
        find_longest_series(&test);
    }
    println!("Elapsed time in microseconds: {} µs", before.elapsed().as_micros());

}

fn main() {
    let tests: Vec<Vec<i32>> = vec![
        vec![5, 15, 14, 13, 12, 16, 10],
        vec![5],
        vec![],
        vec![1,2,3],
        vec![3,2,1],
        vec![3,2,1,2,3],
        vec![1,2,1],
        vec![1,2,2,2,1],
        vec![5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5],
        vec![5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5, 5],
        vec![5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5, 5, 5],
        vec![5, 5, 5, 5, 5, 16, 10],
        vec![5, -15, -14, -13, -12, 16, 10],
        vec![5, -12, -13, -14, -15, 16, 10],
        vec![5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 11, 10, 15, 14, 13, 12, 11, 10, 9, -10, -20],
        vec![5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 11, 10, 15, 14, 13, 12, 11, 10, 9, -10, -20, 2, 3],
        vec![5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 13, 12, 11, 10, 9, -10, -20, 11, 10, 15, 14]
    ];
    println!("Challenge accepted!");

    for test in &tests {
        println!("{test:?}");
        let res = find_longest_series(&test);
        println!("res [{}, {}]  len: {}", res.start, res.stop, res.len);
    }
    
    for _ in 0..10 {
        benchmark();
    }
}
