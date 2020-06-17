
/* Generate a background */
static void
generate_background(Display *dpy, struct lock *lock)
{
        XImage *img;

        int width, height;
        width = DisplayWidth(dpy, lock->screen);
        height = DisplayHeight(dpy, lock->screen);

        img = XGetImage(dpy, lock->root, 0, 0, width, height, AllPlanes, ZPixmap);

        srand(time(NULL));

        unsigned int bytes_per_line = img->bytes_per_line;
        
        unsigned int basis = (rand() % 32) * 4;

        unsigned int width_in_bytes = width * 4;
        unsigned int height_in_bytes = height * bytes_per_line;
        
        for (unsigned int i = 0; i < width_in_bytes; i += 4) {
                for (unsigned int j = 0; j < height_in_bytes; j += bytes_per_line) {
                        if (j % 128 == 0) basis = (rand() % 32) * 4;

                        unsigned int *dest_pixel = (unsigned int *)&img->data[i + j];

                        unsigned int *src_pixel = (unsigned int *)&img->data[((i + basis) % width_in_bytes) + ((j + basis) % height_in_bytes)];

                        *dest_pixel = *src_pixel;
                }
        }

        Pixmap pixmap = XCreatePixmap(dpy, lock->win, width, height, DefaultDepth(dpy, lock->screen));

        GC gc = XCreateGC(dpy, pixmap, 0, NULL);
        XPutImage(dpy, pixmap, gc, img, 0, 0, 0, 0, width, height);
        
        XSetWindowBackgroundPixmap(dpy, lock->win, pixmap);

        XFreePixmap(dpy, pixmap);
        XFreeGC(dpy, gc);
        XDestroyImage(img);
}


// in lockscreen(...)
// Remove the `| CWBackPixel` in `XCreateWindow` and add a call to `generate_background(...)`

	/* init */
	wa.override_redirect = 1;
	wa.background_pixel = lock->colors[INIT];
	lock->win = XCreateWindow(dpy, lock->root, 0, 0,
	                          DisplayWidth(dpy, lock->screen),
	                          DisplayHeight(dpy, lock->screen),
	                          0, DefaultDepth(dpy, lock->screen),
	                          CopyFromParent,
	                          DefaultVisual(dpy, lock->screen),
	                          CWOverrideRedirect, &wa);

	generate_background(dpy, lock);