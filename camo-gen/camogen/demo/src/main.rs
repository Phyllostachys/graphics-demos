use image::EncodableLayout;
use pix_engine::prelude::*;
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

struct CamogenDemo {
    img: camogen::Texture,
    tid: TextureId,
    r: ThreadRng,
}

impl CamogenDemo {
    fn new(texture_width: usize, texture_height: usize) -> CamogenDemo {
        CamogenDemo {
            img: camogen::Texture::new(texture_width, texture_height),
            tid: TextureId::default(),
            r: rand::thread_rng(),
        }
    }

    fn randomize_texture(&mut self) {
        let sampler = Uniform::new(0u8, 3);
        for y in 0..self.img.height {
            for x in 0..self.img.width {
                self.img
                    .set_pixel(x, y, camogen::Color::from(sampler.sample(&mut self.r)));
            }
        }
    }
}

impl AppState for CamogenDemo {
    // Set up application state and initial settings. `PixState` contains
    // engine specific state and utility methods for actions like getting mouse
    // coordinates, drawing shapes, etc. (Optional)
    fn on_start(&mut self, s: &mut PixState) -> PixResult<()> {
        self.randomize_texture();
        self.tid = s.create_texture(
            self.img.width as u32,
            self.img.height as u32,
            PixelFormat::Rgba,
        )?;

        s.clear()
    }

    // Main update/render loop. Called as often as possible unless
    // `target_frame_rate` was set with a value. (Required)
    fn on_update(&mut self, s: &mut PixState) -> PixResult<()> {
        let mut gen_camo = true;

        if s.key_pressed() {
            if s.key_down(Key::R) {
                self.randomize_texture();
            } else if s.key_down(Key::Q) {
                s.close_window(s.window_id())?;
            }
        } else if s.mouse_down(Mouse::Left) {
            let pos = s.mouse_pos();
            let x = (pos.x() as usize) >> 1;
            let y = (pos.y() as usize) >> 1;
            let c = camogen::Color::from(random!(1, 4));

            // println!("{x} {y} {c:?}");

            self.img.set_pixel(x - 2, y - 2, c);
            self.img.set_pixel(x - 2, y - 1, c);
            self.img.set_pixel(x - 2, y, c);
            self.img.set_pixel(x - 2, y + 1, c);
            self.img.set_pixel(x - 2, y + 2, c);

            self.img.set_pixel(x - 1, y - 2, c);
            self.img.set_pixel(x - 1, y - 1, c);
            self.img.set_pixel(x - 1, y, c);
            self.img.set_pixel(x - 1, y + 1, c);
            self.img.set_pixel(x - 1, y + 2, c);

            self.img.set_pixel(x, y - 2, c);
            self.img.set_pixel(x, y - 1, c);
            self.img.set_pixel(x, y, c);
            self.img.set_pixel(x, y + 1, c);
            self.img.set_pixel(x, y + 2, c);

            self.img.set_pixel(x + 1, y - 2, c);
            self.img.set_pixel(x + 1, y - 1, c);
            self.img.set_pixel(x + 1, y, c);
            self.img.set_pixel(x + 1, y + 1, c);
            self.img.set_pixel(x + 1, y + 2, c);

            self.img.set_pixel(x + 2, y - 2, c);
            self.img.set_pixel(x + 2, y - 1, c);
            self.img.set_pixel(x + 2, y, c);
            self.img.set_pixel(x + 2, y + 1, c);
            self.img.set_pixel(x + 2, y + 2, c);

            gen_camo = false;
        }

        if gen_camo {
            let mut temp = camogen::Texture::new(self.img.width, self.img.height);

            for y in 0..self.img.height {
                for x in 0..self.img.width {
                    let new_color = camogen::sample_fanbase(
                        &self.img,
                        x,
                        y,
                        self.img.get_pixel(x, y).into(),
                        &mut self.r,
                    );
                    temp.set_pixel(x, y, new_color);
                }
            }

            self.img = temp;
        }

        s.update_texture(
            self.tid,
            None,
            self.img.data_buffer.as_bytes(),
            self.img.width * 4,
        )?;
        s.texture(self.tid, None, None)?;

        Ok(())
    }

    // Clean up any state or resources before exiting such as deleting temporary
    // files or saving game state. (Optional)
    fn on_stop(&mut self, _s: &mut PixState) -> PixResult<()> {
        Ok(())
    }
}

fn main() -> PixResult<()> {
    let progargs = ProgArgs::from_args();
    // let width = progargs.width;
    // let height = progargs.height;
    let mut demo = CamogenDemo::new(progargs.twidth as usize, progargs.theight as usize);

    let mut engine = PixEngine::builder()
        .with_dimensions(progargs.twidth * 2, progargs.theight * 2)
        .with_title("CamogenDemo")
        .with_frame_rate()
        .target_frame_rate(60)
        .resizable()
        .build()?;
    engine.run(&mut demo)
}
