delacorte
=========

My attempt at a solver for Al Zimmermann's programming contest: Delacorte Numbers, which can be found at http://azspcs.com/Contest/DelacorteNumbers

My first time participating, and my first time doing simulated annealing, so I'm very happy with 24th place. Excuse my messy code. After trying a lot of annealing schedules I ended up with an adaptive one, described in:

> Mathias Ortner, Xavier Descombes, Josiane Zerubia. An adaptive simulated annealing cooling schedule for object detection in images. https://hal.inria.fr/inria-00181764v5

I did not find the O(1) swap algorithm described by whitefox at http://dxdy.ru/topic88308-90.html, which was used by several others. Not sure how much it would have helped.
