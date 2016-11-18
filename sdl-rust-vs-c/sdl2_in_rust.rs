#![feature(libc)]
extern crate libc;

extern crate sdl2_sys;
use sdl2_sys::sdl;
use sdl2_sys::video;
use sdl2_sys::render;
use sdl2_sys::timer;

pub fn main() {
    unsafe {
        let mut res: libc::c_int;
        let mut window: *mut video::SDL_Window = 0 as *mut video::SDL_Window;
        let mut renderer: *mut render::SDL_Renderer = 0 as *mut render::SDL_Renderer;

        println!("Initializing SDL...");
        res = sdl::SDL_Init(sdl::SDL_INIT_EVERYTHING);
        if res != 0 {
            println!("Could not init SDL");
            std::process::exit(-1);
        }

        println!("Creating Window...");
        let title: *const u8 = b"Rust SDL2 Test\0" as *const u8;
        window = video::SDL_CreateWindow(title as *const i8, video::SDL_WINDOWPOS_UNDEFINED, video::SDL_WINDOWPOS_UNDEFINED, 400, 400, video::SDL_WindowFlags::SDL_WINDOW_OPENGL as u32);
        if window == (0 as *mut video::SDL_Window) {
            println!("Could not create window");
            std::process::exit(-1);
        }

        println!("Creating Renderer...");
        renderer = render::SDL_CreateRenderer(window, -1, 0);
        if renderer == (0 as *mut render::SDL_Renderer) {
            println!("Could not create renderer");
            std::process::exit(-1);
        }

        println!("Presenting...");
        render::SDL_RenderPresent(renderer);

        println!("Delaying...");
        timer::SDL_Delay(5000);

        render::SDL_DestroyRenderer(renderer);
        video::SDL_DestroyWindow(window);
    }

    std::process::exit(0);
}
