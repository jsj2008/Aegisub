// Copyright (c) 2013, Thomas Goyne <plorkyeran@aegisub.org>
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
// Aegisub Project http://www.aegisub.org/

/// @file video_out_gl.h
/// @brief OpenGL based video renderer
/// @ingroup video
///

#include <libaegisub/exception.h>

#include <vector>

struct VideoFrame;

/// @class VideoOutGL
/// @brief OpenGL based video renderer
class VideoOutGL {
	struct TextureInfo;

	/// The maximum texture size supported by the user's graphics card
	int maxTextureSize;
	/// Whether rectangular textures are supported by the user's graphics card
	bool supportsRectangularTextures;
	/// The internalformat to use
	int internalFormat;

	/// The frame height which the texture grid has been set up for
	int frameWidth;
	/// The frame width which the texture grid has been set up for
	int frameHeight;
	/// The frame format which the texture grid has been set up for
	GLenum frameFormat;
	/// Whether the grid is set up for flipped video
	bool frameFlipped;
	/// List of OpenGL texture ids used in the grid
	std::vector<GLuint> textureIdList;
	/// List of precalculated texture display information
	std::vector<TextureInfo> textureList;
	/// OpenGL display list which draws the frames
	GLuint dl;
	/// The total texture count
	int textureCount;
	/// The number of rows of textures
	int textureRows;
	/// The number of columns of textures
	int textureCols;

	void DetectOpenGLCapabilities();
	void InitTextures(int width, int height, GLenum format, int bpp, bool flipped);

	VideoOutGL(const VideoOutGL &);
	VideoOutGL& operator=(const VideoOutGL&);
public:
	/// @brief Set the frame to be displayed when Render() is called
	/// @param frame The frame to be displayed
	void UploadFrameData(VideoFrame const& frame);

	/// @brief Render a frame
	/// @param x Bottom left x coordinate
	/// @param y Bottom left y coordinate
	/// @param width Width in pixels of viewport
	/// @param height Height in pixels of viewport
	void Render(int x, int y, int width, int height);

	/// @brief Constructor
	VideoOutGL();
	/// @brief Destructor
	~VideoOutGL();
};

/// @class VideoOutException
/// @extends Aegisub::Exception
/// @brief Base class for all exceptions thrown by VideoOutGL
DEFINE_BASE_EXCEPTION_NOINNER(VideoOutException, agi::Exception)

/// @class VideoOutRenderException
/// @extends VideoOutException
/// @brief An OpenGL error occurred while uploading or displaying a frame
class VideoOutRenderException : public VideoOutException {
public:
	VideoOutRenderException(const char *func, int err)
	: VideoOutException(std::string(func) + " failed with error code " + std::to_string(err))
	{ }
	const char * GetName() const { return "videoout/opengl/render"; }
	Exception * Copy() const { return new VideoOutRenderException(*this); }
};
/// @class VideoOutOpenGLException
/// @extends VideoOutException
/// @brief An OpenGL error occurred while setting up the video display
class VideoOutInitException : public VideoOutException {
public:
	VideoOutInitException(const char *func, int err)
	: VideoOutException(std::string(func) + " failed with error code " + std::to_string(err))
	{ }
	VideoOutInitException(const char *err) : VideoOutException(err) { }
	const char * GetName() const { return "videoout/opengl/init"; }
	Exception * Copy() const { return new VideoOutInitException(*this); }
};
