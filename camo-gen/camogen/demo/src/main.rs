use camogen::*;

use rand::distributions::{Distribution, Uniform};
use rand::rngs::ThreadRng;
use structopt::StructOpt;

#[derive(StructOpt)]
struct ProgArgs {
    // #[structopt(short, long, default_value = "800")]
    // width: u32,

    // #[structopt(short, long, default_value = "600")]
    // height: u32,
    #[structopt(short = "w", long, default_value = "300")]
    twidth: u32,

    #[structopt(short = "h", long, default_value = "225")]
    theight: u32,
}

fn main() {
    let mut rng = rand::thread_rng();
    let progargs = ProgArgs::from_args();
    // let width = progargs.width;
    // let height = progargs.height;
    let tex_width = progargs.twidth as usize;
    let tex_height = progargs.theight as usize;

    let mut img = Texture::new(tex_width, tex_height);
    randomize_texture(&mut img, &mut rng);

    for _ in 0..50 {
        let mut temp = Texture::new(tex_width, tex_height);

        for y in 0..tex_height {
            for x in 0..tex_width {
                let new_color = sample_fanbase(&img, x, y, img.get_pixel(x, y).into(), &mut rng);
                temp.set_pixel(x, y, new_color);
            }
        }

        img = temp;
    }

    let png: image::RgbaImage =
        match image::ImageBuffer::from_raw(tex_width as u32, tex_height as u32, img.data_buffer) {
            Some(i) => i,
            None => image::ImageBuffer::new(tex_width as u32, tex_height as u32),
        };
    png.save("test.png").unwrap();
}

fn randomize_texture(t: &mut Texture, r: &mut ThreadRng) {
    let sampler = Uniform::new(0u8, 3);
    for y in 0..t.height {
        for x in 0..t.width {
            t.set_pixel(x, y, Color::from(sampler.sample(r)));
        }
    }
}
