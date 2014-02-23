#include "ruby.h"
#include <unistd.h>
#include <sys/wait.h>

VALUE eject_cdrom_c(VALUE self)
{
    int status;
    if(fork() == 0)
    {
        execlp("eject", "cdrom", NULL);
    }
    wait(&status);
    if(status == 0)
        return rb_str_new2("ejected...");
    else
        return rb_str_new2("can't eject...");
}

void Init_eject_cdrom()
{
    VALUE demo = rb_define_module("Demo");
    VALUE eject = rb_define_class_under(demo, "Eject", rb_cObject);
    rb_define_singleton_method(eject, "eject_cdrom", eject_cdrom_c, 0);
}
