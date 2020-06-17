void
gaplessgrid(Monitor *m) {
	unsigned int n, cols, rows, cn, rn, i, cx, cy, cw, ch;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) ;
	if(n == 0)
		return;
	if(n == 1){
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	/* grid dimensions */
	for(cols = 0; cols <= n/2; cols++)
		if(cols*cols >= n)
			break;
	if(n == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
		cols = 2;
	rows = n/cols;

	/* window geometries */
	cw = cols ? m->ww / cols : m->ww;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for(i = 0, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if(i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = rows ? m->wh / rows : m->wh;
		cx = m->wx + cn*cw;
		cy = m->wy + rn*ch;
		if(cn+1 == cols && rn+1 == rows)
			resize(c, cx + m->gappx, cy + m->gappx, cw - 2 * (c->bw + m->gappx), ch - 2 * (c->bw + m->gappx), False);
		else if(cn+1 == cols)
			resize(c, cx + m->gappx, cy + m->gappx, cw - 2 * (c->bw + m->gappx), ch - 2 * (c->bw) - m->gappx, False);
		else if(rn+1 == rows)
			resize(c, cx + m->gappx, cy + m->gappx, cw - 2 * (c->bw) - m->gappx, ch - 2 * (c->bw + m->gappx), False);
		else
			resize(c, cx + m->gappx, cy + m->gappx, cw - 2 * (c->bw) - m->gappx, ch - 2 * (c->bw) - m->gappx, False);
		rn++;
		if(rn >= rows) {
			rn = 0;
			cn++;
		}
	}
}
