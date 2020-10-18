
/*
 * File:   FBO.h
 * Author: Frantisek Brablik
 *
 * Created on 12. dubna 2019, 23:36
 */

#pragma once

#ifndef FBO_H
#define FBO_H

#include "utils/graphics.h"

class FBO {
public:
	FBO(int width, int height) {
		glCreateFramebuffers(1, &framebuffer);
		glCreateTextures(GL_TEXTURE_2D, 1, &tex_color);
		glTextureStorage2D(tex_color, 1, GL_RGBA32F, width, height);
		glCreateTextures(GL_TEXTURE_2D, 1, &tex_depth);
		glTextureStorage2D(tex_depth, 1, GL_DEPTH_COMPONENT32F, width, height);
		const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
		glNamedFramebufferDrawBuffers(framebuffer, 1, draw_buffers);
		glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, tex_color, 0);
		glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, tex_depth, 0);
	}
	~FBO() {
		glDeleteFramebuffers(1, &framebuffer);
		glDeleteTextures(1, &tex_color);
		glDeleteTextures(1, &tex_depth);
	}
    void clearColor(float r, float g, float b, float a) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
    }
	void bind() { glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); }
	void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	void clear() {
//		float color[4] = { 0.0, 0.0, 0.0, 1.0 };
		float depth[1] = { 1.0 };
		glClearNamedFramebufferfv(framebuffer, GL_COLOR, 0, color);
		glClearNamedFramebufferfv(framebuffer, GL_DEPTH, 0, depth);
	}
	void resize(int width, int height) {
		glDeleteTextures(1, &tex_color);
		glDeleteTextures(1, &tex_depth);
		glCreateTextures(GL_TEXTURE_2D, 1, &tex_color);
		glTextureStorage2D(tex_color, 1, GL_RGBA32F, width, height);
		glCreateTextures(GL_TEXTURE_2D, 1, &tex_depth);
		glTextureStorage2D(tex_depth, 1, GL_DEPTH_COMPONENT32F, width, height);
		const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
		glNamedFramebufferDrawBuffers(framebuffer, 1, draw_buffers);
		glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, tex_color, 0);
		glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, tex_depth, 0);
	}
	GLuint color_texture() { return tex_color; }
	GLuint depth_texture() { return tex_depth; }
private:
	GLuint framebuffer = 0;
	GLuint tex_color = 0;
	GLuint tex_depth = 0;
    float color[4] = { 0.0, 0.0, 0.0, 1.0 };
};

#endif /* FBO_H */

