// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.buffer;

import wing.graphics;

export namespace wing {

class Buffer
{
public:
    Buffer(const Size& size_, Graphics& graphics_);
    const Size& GetSize() const { return size; }
    Graphics& BitmapGraphics() { return bmgraphics; }
    Bitmap& GetBitmap() { return bitmap; }
    void Draw(Graphics& graphics);
private:
    Size size;
    Bitmap bitmap;
    Graphics bmgraphics;
};

} // wing
