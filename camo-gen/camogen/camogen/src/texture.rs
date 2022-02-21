#[derive(Debug)]
pub struct Color {
    pub red: u8,
    pub green: u8,
    pub blue: u8,
    pub alpha: u8,
}

pub struct Texture {
    pub data_buffer: Vec<u8>,
    pub width: usize,
    pub height: usize,
}

impl Texture {
    pub fn new(width: usize, height: usize) -> Texture {
        let mut data = Vec::new();
        data.resize((width * height * 4) as usize, 0);
        Texture {
            data_buffer: data,
            width,
            height,
        }
    }

    pub fn set_pixel(&mut self, x: usize, y: usize, c: Color) {
        let mut offset = (y * (self.width * 4)) + (x * 4);
        if offset >= self.data_buffer.len() - 4 {
            offset = self.data_buffer.len() - 4;
        }
        self.data_buffer[offset] = c.red;
        self.data_buffer[offset + 1] = c.green;
        self.data_buffer[offset + 2] = c.blue;
        self.data_buffer[offset + 3] = c.alpha;
    }

    pub fn get_pixel(&self, x: usize, y: usize) -> Color {
        let mut offset = (y * (self.width * 4)) + (x * 4);
        if offset >= self.data_buffer.len() - 4 {
            offset = self.data_buffer.len() - 4;
        }
        Color {
            red: self.data_buffer[offset],
            green: self.data_buffer[offset + 1],
            blue: self.data_buffer[offset + 2],
            alpha: self.data_buffer[offset + 3],
        }
    }
}
