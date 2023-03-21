static void
monocle(Monitor *m)
{
	unsigned int n = 0;
	Client *c;

	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;
	if (n > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
}

static void
mastergrid(Monitor *m) {
	int i, gap;
	unsigned int n, cols, rows, cn, rn, cx, cy, cw, ch, mw, mh, my;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
	/* dynamic gapps if there are more than 4 slaves */
	gap = (n > 4) ? (gappx - 2*(n - 4)) : gappx;
	/* If gap is negative force it to 0 */
	gap = (gap < 0) ? 0 : gap;
	/* vertical monitor, vertically stacked windows */
	if (m->mh > m->mw)
		m->nmaster = 0;

	/* if one window or nmaster 0 then full width master window */
	if (n <= 1 || m->nmaster == 0) {
		mw = m->ww;

	/* scaled master window */
	} else {
		/* scale master window's width */
		mw = m->ww*m->mfact;
		/* ignore master window's count to calculate slave grids */
		n--;
	}
	/* Calculate how many rows required */
	for (rows = 0; rows*rows < n; rows++);
	/* set tiling layout until we have more than 3 slaves */
	if (n == 3)
		rows = 3;

	cols = n/rows;
	/* window geometries */
	ch = rows ? (m->wh-gap) / rows : m->wh;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	my = gap; /* master y coord. */
	for (i = -1, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if (i == -1 || m->nmaster == 0) {
			mh = (m->wh - my) / ((m->nmaster ? 1 : n) - i - 1);
			resize(c, m->wx + gap, m->wy + my,
				mw - 2*c->bw - 2*gap, mh - 2*c->bw - gap,0);
			my += HEIGHT(c) + gap;
			continue;
		}
		if (i/cols + 1 > rows - n%rows)
			cols = n/rows + 1;
		cw = cols ? (m->ww - mw)/cols : (m->ww - mw) - gap;
		cx = m->wx + cn*cw + mw;
		cy = m->wy + rn*ch;
		resize(c, cx, cy + gap, cw - 2*c->bw - gap,
			ch - 2*c->bw - gap, 0);
		cn++;
		if (cn >= cols) {
			cn = 0;
			rn++;
		}
	}
}

/* There is gappx implemented by gg. Rest is gappless grid patch. */
static void
gapfulgrid(Monitor *m) {
	unsigned int n, cols, rows, cn, rn, i, cx, cy, cw, ch;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) ;
	if(n == 0)
		return;

	/* grid dimensions */
	for (cols = 0; cols*cols < n; cols++);
	rows = n/cols;

	/* window geometries */
	cw = cols ? (m->ww-gappx) / cols : m->ww;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for(i = 0, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if(i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = rows ? (m->wh-gappx) / rows : m->wh;
		cx = m->wx + cn*cw;
		cy = m->wy + rn*ch;
		resize(c,
			cx + gappx,
			cy + gappx,
			cw - 2 * c->bw - gappx,
			ch - 2 * c->bw - gappx,
			False);
		rn++;
		if(rn >= rows) {
			rn = 0;
			cn++;
		}
	}
}

#ifdef extra_layouts

static void
vertmastergrid(Monitor *m) {
	int i;
	unsigned int n, cols, rows, cn, rn, cx, cy, cw, ch, mw;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) ;
	if (n == 0)
		return;

	if (n > 1) {
		/* scale master window's width */
		mw = m->ww * m->mfact;
		/* reduce number windows for calculating slave grids */
		n--;
	} else
		mw = m->ww;
	/* Calculate how many columns are required */
	for (cols = 0; cols*cols < n; cols++);
	rows = n/cols;
	/* window geometries */
	cw = cols ? (m->ww - mw) / cols : (m->ww - mw);
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for (i = -1, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if (i == -1) {
			cy = 0; //gappx;
			resize(c,
				m->wx, //+ gappx,
				m->wy + cy,
				mw - (2*c->bw),// - 2*gappx,
				m->wh - cy - (2*c->bw), //- gappx,
				0);
			continue;
		}
		if (i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = rows ? m->wh / rows : m->wh;
		cx = m->wx + cn*cw + mw;
		cy = m->wy + rn*ch;
		resize(c, cx, cy, cw - 2 * c->bw, ch - 2 * c->bw, False);
		rn++;
		if (rn >= rows) {
			rn = 0;
			cn++;
		}
	}
}

static void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? m->ww * m->mfact : gappx;
	else
		mw = m->ww;
	for (i = 0, my = ty = gappx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i) - gappx;
			resize(c, m->wx + gappx, m->wy + my, mw - (2*c->bw) - 2*gappx, h - (2*c->bw), 0);
			if (my + HEIGHT(c) < m->wh)
				my += HEIGHT(c) + gappx;
		} else {
			h = (m->wh - ty) / (n - i) - gappx;
			resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw) - gappx, h - (2*c->bw), 0);
			if (ty + HEIGHT(c) < m->wh)
				ty += HEIGHT(c) + gappx;
		}
}
#endif
