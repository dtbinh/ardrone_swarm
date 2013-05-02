#include "CDecoder.h"

CDecoder::CDecoder()
{
	avcodec_register_all();
	av_init_packet(&avpkt);
	codec = avcodec_find_decoder(CODEC_ID_H264);
	if (!codec) {
		fprintf(stderr, "codec not found\n");
		exit(1);
	}
	c = avcodec_alloc_context3(codec);
	picture = avcodec_alloc_frame();
	pictureRGB = avcodec_alloc_frame();
	srcX=dstX = 640;
	srcY=dstY = 368;
	img_convert_ctx = sws_getContext(srcX, srcY,PIX_FMT_YUV420P, dstX, dstY, PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);


//	if(codec->capabilities&CODEC_CAP_TRUNCATED) c->flags|= CODEC_FLAG_TRUNCATED; 
	if (avcodec_open2(c, codec, NULL) < 0) {
		fprintf(stderr, "could not open codec\n");
		exit(1);
	}
}

CDecoder::~CDecoder()
{
	sws_freeContext(img_convert_ctx); 
	avcodec_close(c);
	av_free(c);
	av_free(picture);
}

int CDecoder::decode(unsigned char* buf,int len,CRawImage* im)
{
	avpkt.size = len;
	avpkt.data = buf;
	avcodec_decode_video2(c, picture, &got_picture, &avpkt);
	if (got_picture == 360){
		avpicture_fill((AVPicture *)pictureRGB,im->data,PIX_FMT_RGB24, dstX, dstY);
		sws_scale(img_convert_ctx, picture->data, picture->linesize, 0, srcY,pictureRGB->data, pictureRGB->linesize);
	}
	if (got_picture == 360) return 0;
	return 1;
}


