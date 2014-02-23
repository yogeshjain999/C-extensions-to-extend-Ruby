#include "ruby.h"
#include <opencv/highgui.h>
#include <opencv/cv.h>

static VALUE take_photo_c(VALUE self, VALUE index, VALUE img_name)
{
    IplImage *img = 0;

    // capture and store image from given cam index
    CvCapture* capture = cvCaptureFromCAM(FIX2INT(index));

    if(!cvGrabFrame(capture))
    {
        cvReleaseCapture(&capture);
        return rb_str_new2("Error while grabing frame");
    }
    img = cvRetrieveFrame(capture, 0);
    if(!img)
    {
        cvReleaseCapture(&capture);
        return rb_str_new2("Error while retriving frame");
    }
    if(!cvSaveImage(RSTRING_PTR(img_name), img, 0))
    {
        cvReleaseCapture(&capture);
        return rb_str_new2("Cant save image");
    }
    cvReleaseCapture(&capture);
    return rb_str_new2("Image has been created");
}

static VALUE list_devises_c(VALUE self)
{
    int i;
    ID entries_id, read_id;
    VALUE path, list = rb_ary_new();

    // get list of directories from /sys/class/video4linux
    entries_id = rb_intern("entries");
    list = rb_funcall(rb_cDir, entries_id, 1, rb_str_new2("/sys/class/video4linux"));

    // iterate all entries from /sys/class/video4linux
    for(i = 2; i < RARRAY_LEN(list); i++)
    {
        // generate path to get device's name in the /sys/class/video4linux
        path = rb_str_cat2( rb_str_new2("/sys/class/video4linux/"), RSTRING_PTR(rb_ary_entry(list, i)));
        path = rb_str_cat2(path, "/name");

        // read name of the device from file
        read_id = rb_intern("read");
        printf("%c\t%s", RSTRING_PTR(rb_ary_entry(list,i))[5], RSTRING_PTR(rb_funcall(rb_cIO, read_id,1, path)));
    }
    return Qnil;
}

void Init_webcam()
{
    
    VALUE capture = rb_define_class("Capture", rb_cObject);close(2);
    rb_define_method(capture, "take_photo", take_photo_c, 2);
    rb_define_method(capture, "list_devises", list_devises_c, 0);
}
