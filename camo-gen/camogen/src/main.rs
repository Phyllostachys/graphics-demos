mod texture;
use texture::{Color, Texture};

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

#[derive(PartialEq, Eq, Debug)]
enum Pixel {
    Black,
    Green,
    Brown,
}

impl From<u8> for Pixel {
    fn from(v: u8) -> Self {
        match v {
            1 => Pixel::Green,
            2 => Pixel::Brown,
            _ => Pixel::Black,
        }
    }
}

impl From<Pixel> for Color {
    fn from(p: Pixel) -> Self {
        match p {
            Pixel::Black => Color {
                red: 0,
                green: 0,
                blue: 0,
                alpha: 255,
            },
            Pixel::Green => Color {
                red: 107,
                green: 142,
                blue: 35,
                alpha: 255,
            },
            Pixel::Brown => Color {
                red: 135,
                green: 74,
                blue: 43,
                alpha: 255,
            },
        }
    }
}

impl From<u8> for Color {
    fn from(v: u8) -> Self {
        let p: Pixel = v.into();
        p.into()
    }
}

impl From<Color> for Pixel {
    fn from(c: Color) -> Self {
        if c.red == 107 && c.green == 142 && c.blue == 35 {
            Pixel::Green
        } else if c.red == 135 && c.green == 74 && c.blue == 43 {
            Pixel::Brown
        } else {
            Pixel::Black
        }
    }
}

fn sample_fanbase(i: &Texture, x: usize, y: usize, color: Pixel, r: &mut ThreadRng) -> Color {
    let sampler = Uniform::new(0u8, 3);
    let new_color: Color;
    let surroundings = get_surround_counts(i, x, y);

    match color {
        Pixel::Black => {
            if surroundings.black >= 4 {
                new_color = Pixel::Black.into();
            } else if surroundings.green == surroundings.brown {
                new_color = Color::from(sampler.sample(r));
            } else if surroundings.green > surroundings.brown {
                new_color = Pixel::Green.into();
            } else {
                new_color = Pixel::Brown.into();
            }
        }
        Pixel::Green => {
            if surroundings.green >= 4 {
                new_color = Pixel::Green.into();
            } else if surroundings.black == surroundings.brown {
                new_color = Color::from(sampler.sample(r));
            } else if surroundings.black > surroundings.brown {
                new_color = Pixel::Black.into();
            } else {
                new_color = Pixel::Brown.into();
            }
        }
        Pixel::Brown => {
            if surroundings.brown >= 4 {
                new_color = Pixel::Brown.into();
            } else if surroundings.green == surroundings.black {
                new_color = Color::from(sampler.sample(r));
            } else if surroundings.green > surroundings.black {
                new_color = Pixel::Green.into();
            } else {
                new_color = Pixel::Black.into();
            }
        }
    }

    new_color
}

struct SurroundCount {
    black: u32,
    green: u32,
    brown: u32,
}

fn get_surround_counts(i: &Texture, x: usize, y: usize) -> SurroundCount {
    SurroundCount {
        black: search_for_similar(i, x, y, Pixel::Black),
        green: search_for_similar(i, x, y, Pixel::Green),
        brown: search_for_similar(i, x, y, Pixel::Brown),
    }
}

fn search_for_similar(i: &Texture, x: usize, y: usize, color: Pixel) -> u32 {
    let mut result = 0;

    let mut left = x.saturating_sub(1);
    let mut right = x + 1;
    let mut top = y.saturating_sub(1);
    let mut bottom = y + 1;

    if x == 0 {
        left = i.width - 1;
    }
    if x == i.width {
        right = 0;
    }
    if y == 0 {
        top = i.height - 1;
    }
    if y == i.height {
        bottom = 0;
    }

    // top
    if Pixel::from(i.get_pixel(left, top)) == color {
        result += 1;
    }
    if Pixel::from(i.get_pixel(x, top)) == color {
        result += 1;
    }
    if Pixel::from(i.get_pixel(right, top)) == color {
        result += 1;
    }

    // left and right
    if Pixel::from(i.get_pixel(left, y)) == color {
        result += 1;
    }
    if Pixel::from(i.get_pixel(right, y)) == color {
        result += 1;
    }

    // bottom
    if Pixel::from(i.get_pixel(left, bottom)) == color {
        result += 1;
    }
    if Pixel::from(i.get_pixel(x, bottom)) == color {
        result += 1;
    }
    if Pixel::from(i.get_pixel(right, bottom)) == color {
        result += 1;
    }

    result
}
