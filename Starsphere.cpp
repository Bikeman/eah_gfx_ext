#include "Starsphere.h"

Starsphere::Starsphere()
{
	Axes=0, Stars=0, Constellations=0, Pulsars=0;
	LLOmarker=0, LHOmarker=0, GEOmarker=0;
	sphGrid=0, SNRs=0;

	/**
	 * Parameters and State info
	 */
	sphRadius = 5.5;
	featureFlags = 0;

	/**
	 * Viewpoint (can be changed with mouse)
	 */
	viewpt_azimuth = 30.0;
	viewpt_elev = 23.6;
	viewpt_radius = 7.6;

	wobble_amp = 37.0;
	wobble_period = 17.0;
	zoom_amp = 0.00;
	zoom_period = 29.0;

	rotation_offset = 0.0;
	rotation_speed = 180.0;

	gfx_width = 0;
	gfx_height = 0;

	/* Time info */
	gmt_offset=0.0;
	show_gmt=true;

	// Graphics state info:
	aspect = 4 / 3;
}

Starsphere::~Starsphere()
{
	if(m_PolygonFont) delete m_PolygonFont;
}

/**
 * sphVertex3D() creates a GL vertex in 3D sky sphere coordinates
 * sphVertex() creates a GL vertex on the surface of the sky sphere.
 * Use either like glVertex().
 */
void Starsphere::sphVertex3D(GLfloat RAdeg, GLfloat DEdeg, GLfloat radius)
{
	GLfloat x, y, z;

	x = radius * COS(DEdeg) * COS(RAdeg);
	z = -radius * COS(DEdeg) * SIN(RAdeg);
	y = radius * SIN(DEdeg);
	glVertex3f(x, y, z);
	return;
}

void Starsphere::sphVertex(GLfloat RAdeg, GLfloat DEdeg)
{
	sphVertex3D(RAdeg, DEdeg, sphRadius);
}

/**
 * Star Marker:
 * Makes a marker for one star at a given position and angular size.  
 */
void Starsphere::star_marker(float RAdeg, float DEdeg, float size)
{
	glPointSize((GLfloat) size);
	glBegin(GL_POINTS);
	sphVertex((GLfloat) RAdeg, (GLfloat) DEdeg);
	glEnd();
	return;
}

/**
 *  Create Stars: markers for each star
 */
void Starsphere::make_stars()
{
	GLfloat mag_size;
	int i, j;
	bool is_dupe;
	int Ndupes=0;

	if (!Stars)
		Stars = glGenLists(1);
	glNewList(Stars, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);

	/**
	 * At some point in the future star_info[][] will also contain
	 * star magnitude and the marker size will vary with this.
	 */
	for (i=0; i < Nstars; i++) {
		// same stars appear more than once in constallations so ignore dupes
		is_dupe=false;
		for (j=0; j< i; j++) {
			if (star_info[j][0] == star_info[i][0] && star_info[j][0]
			        == star_info[i][0]) {
				is_dupe=true;
				Ndupes++;
				break;
			}
		}
		if (!is_dupe) {
			// mag_size = 0.05 + 0.50*rand()/RAND_MAX;
			mag_size = 4.0;
			star_marker(star_info[i][0], star_info[i][1], mag_size);
		}
	}
	glEndList();
	// fprintf(stderr,"make_stars: there were %d duplicate stars.\n", Ndupes);
}

/**
 *  Pulsar Markers:
 */
void Starsphere::make_pulsars()
{
	GLfloat mag_size=3.0;
	int i;

	if (!Pulsars)
		Pulsars = glGenLists(1);
	glNewList(Pulsars, GL_COMPILE);
	glColor3f(0.80, 0.0, 0.85); // _P_ulsars are _P_urple

	for (i=0; i < Npulsars; i++) {
		star_marker(pulsar_info[i][0], pulsar_info[i][1], mag_size);
	}
	glEndList();
}

/**
 * Super Novae Remenants (SNRs):
 */
void Starsphere::make_snrs()
{
	GLfloat mag_size=3.0;
	int i;

	if (!SNRs)
		SNRs = glGenLists(1);
	glNewList(SNRs, GL_COMPILE);
	glColor3f(0.7, 0.176, 0.0); // _S_NRs are _S_ienna

	for (i=0; i < NSNRs; i++) {
		star_marker(SNR_info[i][0], SNR_info[i][1], mag_size);
	}
	glEndList();
}

/**
 * Create Constellations:
 * draws line links between pairs of stars in the list.
 */
void Starsphere::make_constellations()
{
	GLint star_num=0;

	if (!Constellations)
		Constellations = glGenLists(1);
	glNewList(Constellations, GL_COMPILE);
	glLineWidth(1.0);
	glColor3f(0.7, 0.7, 0.0); // light yellow

	glBegin(GL_LINES); // draws lines between *pairs* of vertices
	for (star_num=0; star_num < Nstars; ++star_num) {
		sphVertex(star_info[star_num][0], star_info[star_num][1]);
		star_num++;
		sphVertex(star_info[star_num][0], star_info[star_num][1]);
	}
	glEnd();
	glEndList();
}

/**
 * Create markers on sky sphere for LLO, LHO, and GEO
 * IFO corner positions are from Myers' personal GPS and are +/- 100m
 */

/**
 * RAofZenith(time, longitude)
 *
 * Computes the Right Ascention of the zenith at a given time (from
 * the Unix epoch, in seconds) at a given Longitude (in degrees). From
 * 'The Cambridge Handbook of Physics Formulas', Graham Woan, 2003
 * edition, CUP.  (NOT the first edition), p177.
 */
GLfloat Starsphere::RAofZenith(double T, GLfloat LONdeg)
{

	// unix epoch at 12h  1/1/2000
	const double T_0 = 946728000.0;

	//  UT seconds of the day
	double T_s = fmod(T, 24.0*3600.0);

	// Julian centuries since 12h 1/1/2000 and 0h today
	double T_c = (T - T_s - T_0)/3155760000.0;

	// GMST at 0h today in seconds
	double GMST0 = 6.0*3600.0 + 41.0*60.0 + 50.54841 + (8640184.812866
	        + 0.093104*T_c)*T_c;

	// GMST now in seconds
	double GMST = GMST0 + 1.002738*T_s;

	// longitude defined as west positive
	GLfloat alpha = (GMST/(24.0*3600.0))*360.0 - LONdeg;

	return alpha;
}

/**
 * Draw the observatories at their zenith positions
 */
void Starsphere::make_obs()
{
	GLfloat Lat, Lon; // Latitute/Longitude of IFO is
	GLfloat RAdeg, DEdeg; // converted to RA/DEC of sky sphere position
	GLfloat radius; // radius of sphere for obs

	GLfloat arm_len_deg=3.000; // lenght of arms, in degrees (not to scale)
	GLfloat h2=0.400; // slight offset for H2 arms

	double obs_gmt_dtime; // current time in GMT to get zenith position

	obs_gmt_dtime = 0.0; //FIXME: gmt_dtime();  // Current GMT time
	obs_dtime_drawn = 0.0; //FIXME:dtime();    // dtime() not gmt_dtime(), for difference
	gmt_offset = obs_gmt_dtime - obs_dtime_drawn; // save for GMT display

	radius = 1.0*sphRadius; // radius of sphere on which they are drawn

	float lineSize = 4.0;

	/**
	 * LIGO Livingston Observatory:
	 */

	Lat= 30.56377;
	Lon= 90.77408;

	RAdeg= RAofZenith(obs_gmt_dtime, Lon);
	DEdeg= Lat;

	if (!LLOmarker)
		LLOmarker = glGenLists(1);
	glNewList(LLOmarker, GL_COMPILE);
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(lineSize);
	glBegin(GL_LINE_STRIP);
	//  North/South arm:
	sphVertex3D(RAdeg, DEdeg-arm_len_deg, radius);
	sphVertex3D(RAdeg, DEdeg, radius);
	// East/West arm:
	sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
	glEnd();
	// arm joint H2
	glPointSize((GLfloat) lineSize);
	glBegin(GL_POINTS);
	sphVertex3D(RAdeg, DEdeg, radius);
	glEnd();
	glEndList();

	/**
	 * LIGO Hanford Observatory: H1 and H2
	 */

	Lat= 46.45510;
	Lon= 119.40627;

	RAdeg= RAofZenith(obs_gmt_dtime, Lon);
	DEdeg= Lat;

	if (!LHOmarker)
		LHOmarker = glGenLists(1);
	glNewList(LHOmarker, GL_COMPILE);
	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(lineSize);
	glBegin(GL_LINE_STRIP);
	// North/South arm:
	sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
	sphVertex3D(RAdeg, DEdeg, radius);
	// East/West arm:
	sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
	glEnd();
	glBegin(GL_LINE_STRIP);
	// North/South arm, H2:
	sphVertex3D(RAdeg-h2, DEdeg+arm_len_deg/2.0+h2/2.0, radius);
	sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
	// East/West arm, H2;
	sphVertex3D(RAdeg-arm_len_deg/2.0-h2, DEdeg+h2/2.0, radius);
	glEnd();
	// arm joint H1
	glPointSize((GLfloat) lineSize);
	glBegin(GL_POINTS);
	sphVertex3D(RAdeg, DEdeg, radius);
	glEnd();
	// arm joint H2
	glPointSize((GLfloat) lineSize);
	glBegin(GL_POINTS);
	sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
	glEnd();
	glEndList();

	/**
	 *  GEO600 Interferometer:
	 */

	Lat= 52.24452;
	Lon= -9.80683;
	arm_len_deg=1.50; // not to scale

	RAdeg= RAofZenith(obs_gmt_dtime, Lon);
	DEdeg= Lat;

	if (!GEOmarker)
		GEOmarker = glGenLists(1);
	glNewList(GEOmarker, GL_COMPILE);
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(lineSize);
	glBegin(GL_LINE_STRIP);
	// North/South arm:
	sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
	sphVertex3D(RAdeg, DEdeg, radius);
	// West/East arm:
	sphVertex3D(RAdeg+arm_len_deg, DEdeg, radius);
	glEnd();
	// arm joint
	glPointSize((GLfloat) lineSize);
	glBegin(GL_POINTS);
	sphVertex3D(RAdeg, DEdeg, radius);
	glEnd();
	glEndList();

	return;
}

/**
 * XYZ coordinate axes: (if we want them - most useful for testing)
 */
void Starsphere::make_axes()
{
	GLfloat axl=10.0;

	if (!Axes)
		Axes = glGenLists(1);
	glNewList(Axes, GL_COMPILE);
	glLineWidth(2.0);

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-axl, 0.0, 0.0);
	glVertex3f(axl, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -axl, 0.0);
	glVertex3f(0.0, axl, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -axl);
	glVertex3f(0.0, 0.0, axl);
	glEnd();
	glEndList();
}

/**
 * RA/DEC coordinate grid on the sphere
 */
void Starsphere::make_globe()
{
	int hr, j, i, iMax=100;
	GLfloat RAdeg, DEdeg;

	if (!sphGrid)
		sphGrid = glGenLists(1);
	glNewList(sphGrid, GL_COMPILE);
	glLineWidth(1.0);

	// Lines of constant Right Ascencion (East Longitude)

	for (hr=0; hr<24; hr++) {
		RAdeg=hr*15.0;
		glColor3f(0.25, 0.25, 0.25);
		if (hr==0)
			glColor3f(0.55, 0.55, 0.55);
		glBegin(GL_LINE_STRIP);
		for (i=0; i<=iMax; i++) {
			DEdeg = i*180.0/iMax - 90.0;
			sphVertex(RAdeg, DEdeg);
		}
		glEnd();
	}

	// Lines of constant Declination (Lattitude)

	for (j=1; j<=12; j++) {
		DEdeg = 90.0 - j*15.0;
		glBegin(GL_LINE_STRIP);
		for (i=0; i<=iMax; i++) {
			RAdeg = i*360.0/iMax;
			sphVertex(RAdeg, DEdeg);
		}
		glEnd();
	}
	glEndList();
}

/**
 * Window resize/remap
 * (FWIW, this is called *before* app_graphics_init(), 
 * when window is first mapped)
 */
void Starsphere::resize(const int width, const int height)
{

	/* Adjust aspect ratio and projection */
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	gfx_width = width;
	gfx_height = height;
	aspect = gfx_width / gfx_height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(95.0, aspect, 0.50, 25.0);
	glMatrixMode(GL_MODELVIEW);

	// Update in case time (zone?) change
	gmt_offset = 2; //FIXME: gmt_dtime() - dtime(); 
}

/**
 *  What to do when graphics are "initialized".
 */
void Starsphere::initialize(const int width, const int height, const Resource *font)
{
	gfx_width = width;
	gfx_height = height;
	aspect = gfx_width / gfx_height;
	this->m_FontResource = font;
	
	// create font instance using font resource (base address + size)
	m_PolygonFont = new FTGLPolygonFont((&m_FontResource->data()->at(0)), m_FontResource->data()->size());
	
	m_PolygonFont->CharMap(ft_encoding_unicode);
//	m_PolygonFont->Depth(0.05);
	m_PolygonFont->FaceSize(1);

	// Drawing setup:
	glClearColor(0.0, 0.0, 0.0, 0.0); // background is black
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	// some polishing
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	
	// FSAA will only be enabled when needed!
	glDisable(GL_MULTISAMPLE_ARB);

	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Enable depth buffering for 3D graphics */
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* Fog aids depth perception */
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, 0.085);

	/* Set initial perspective projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(95.0, aspect, 0.50, 25.0);
	glMatrixMode(GL_MODELVIEW);

	/* Create pre-drawn display lists */
	make_stars();
	make_constellations();
	make_pulsars();
	make_snrs();
	make_axes();
	make_globe();
	make_obs();

	setFeature(STARS, true);
	setFeature(CONSTELLATIONS, true);
	setFeature(PULSARS, true);
	setFeature(OBSERVATORIES, true);
	setFeature(SNRS, true);
	setFeature(GLOBE, true);
	setFeature(SEARCHINFO, true);
	setFeature(LOGO, true);

	glDisable(GL_CLIP_PLANE0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glFlush();
}

/**
 * Rendering routine:  this is what does the drawing:   
 */
void Starsphere::render(const double timeOfDay)
{
	GLfloat xvp, yvp, zvp, vp_theta, vp_phi, vp_rad;
	GLfloat Zrot = 0.0, Zobs=0.0;
	double revs, t, dt = 0;
	static double start_time=-1.0, last_time=-1.0;
	
	// Calculate the real time t since we started (or reset) and the
	// time dt since the last render() call.    Both may be useful
	// for timing animations.  Note that time_of_day is dtime().

	if (start_time < 0.0)
		start_time = timeOfDay;
	t = timeOfDay - start_time;

	if (last_time < 0.0)
		last_time = timeOfDay - 0.01;
	dt = timeOfDay - last_time;

	last_time = timeOfDay; // remember for next time

	// Now determine the rotation angle based on the time since start
	// It is negative to get the rotation direction correct (the sun
	// rises in the East, so the sky sphere rotates E to W).

	Zrot = t*rotation_speed/60.0;
	revs = Zrot/360.0;
	Zrot = -360.0 * (revs - (int)revs);

	// And start drawing...
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Now draw the scene...
	glLoadIdentity();

	// Vary the viewpoint with both a long period wobble of the elevation 
	// of the view and a longer period zoom in/out that might even penetrate
	// The starsphere for a brief time.   Increase the power in pow(,) to 
	// make the visit inside briefer.

	vp_theta = 90.0 - viewpt_elev + wobble_amp*sin(PI2*t/(wobble_period*60.0));
	vp_phi = viewpt_azimuth;
	vp_rad = viewpt_radius - zoom_amp*sin(PI2*t/(zoom_period*60.0));
	if (vp_rad <0.0)
		vp_rad = 0.0; // cannot pass origin (confusing)
	// TRIED THIS TOO: -zoom_amp*pow(fabs(sin(PI2*t/(zoom_period*60.0))),3);
	xvp = vp_rad * SIN(vp_theta) * SIN(vp_phi);
	zvp = vp_rad * SIN(vp_theta) * COS(vp_phi);
	yvp = vp_rad * COS(vp_theta);

	gluLookAt(xvp, yvp, zvp, // eyes position
	        0.0, 0.0, 0.0, // looking toward here
	        0.0, 1.0, 0.0); // which way is up?  y axis!

	// Draw axes before any rotation so they stay put
	if (isFeature(AXES))
		glCallList(Axes);

	// Draw the sky sphere, with rotation:
	glPushMatrix();
	glRotatef(Zrot - rotation_offset, 0.0, 1.0, 0.0);

	/* stars, pulsars, supernovae */

	if (isFeature(STARS))
		glCallList(Stars);
	if (isFeature(PULSARS))
		glCallList(Pulsars);
	if (isFeature(SNRS))
		glCallList(SNRs);
	if (isFeature(CONSTELLATIONS))
		glCallList(Constellations);
	if (isFeature(GLOBE))
		glCallList(sphGrid);

	/* Observatories move an extra 15 degrees/hr since they were drawn */

	if (isFeature(OBSERVATORIES)) {
		glPushMatrix();
		Zobs = (timeOfDay - obs_dtime_drawn) * 15.0/3600.0;
		glRotatef(Zobs, 0.0, 1.0, 0.0);
		glCallList(LLOmarker);
		glCallList(LHOmarker);
		glCallList(GEOmarker);
		glPopMatrix();
	}

	glPopMatrix();

	// draw 2D vectorized HUD
	if(isFeature(LOGO) || isFeature(SEARCHINFO)) {
	
		static GLfloat xStartPosLeft = 0.008;
		static GLfloat xStartPosRight = 0.84;
		static GLfloat yStartPosTop = 0.9675;
		static GLfloat yStartPosBottom = 0.075;
		static GLfloat fontScaleLargeX = 0.020;
		static GLfloat fontScaleMediumX = 0.0115;
		static GLfloat fontScaleSmallX = 0.01;
		static GLfloat fontScaleMediumY = fontScaleMediumX * aspect;
		static GLfloat fontScaleLargeY = fontScaleLargeX * aspect;
		static GLfloat fontScaleSmallY = fontScaleSmallX * aspect;
		//    static GLfloat yOffset = (font->Ascender() + font->Descender());
		static GLfloat yOffsetLarge = 0.02;
		static GLfloat yOffsetMedium = 0.015;
	
		// enable FSAA
		glEnable(GL_MULTISAMPLE_ARB);
		
		// save current state
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	
		if (isFeature(LOGO)) {
			glPushMatrix();
			glColor3f(1.0, 1.0, 0.0);
			glTranslatef(xStartPosLeft, yStartPosTop, 0);
			glScalef(fontScaleLargeX, fontScaleLargeY, 1.0);
			m_PolygonFont->Render("Einstein@Home");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glColor4f(1.0, 1.0, 1.0, 0.5);
			glTranslatef(xStartPosLeft, yStartPosTop - yOffsetLarge, 0);
			glScalef(fontScaleMediumX, fontScaleMediumY, 1.0);
			m_PolygonFont->Render("World Year of Physics 2005");
			glPopMatrix();
		}
		if (isFeature(SEARCHINFO)) {
			// left info block      
			glPushMatrix();
			glColor3f(1.0, 1.0, 0.0);
			glTranslatef(xStartPosLeft, yStartPosBottom, 0);
			glScalef(fontScaleMediumX, fontScaleMediumY, 1.0);
			m_PolygonFont->Render("BOINC Statistics");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glColor4f(1.0, 1.0, 1.0, 0.5);
			glTranslatef(xStartPosLeft, yStartPosBottom - yOffsetMedium, 0);
			glScalef(fontScaleSmallX, fontScaleSmallY, 1.0);
			m_PolygonFont->Render("User: Oliver");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(xStartPosLeft, yStartPosBottom - 2*yOffsetMedium, 0);
			glScalef(fontScaleSmallX, fontScaleSmallY, 1.0);
			m_PolygonFont->Render("Team: Albert-Einstein-Institut");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(xStartPosLeft, yStartPosBottom - 3*yOffsetMedium, 0);
			glScalef(fontScaleSmallX, fontScaleSmallY, 1.0);
			m_PolygonFont->Render("Project credits: 12.000");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(xStartPosLeft, yStartPosBottom - 4*yOffsetMedium, 0);
			glScalef(fontScaleSmallX, fontScaleSmallY, 1.0);
			m_PolygonFont->Render("Project RAC: 145.00");
			glPopMatrix();
	
			// right info block
			glPushMatrix();
			glColor3f(1.0, 1.0, 0.0);
			glTranslatef(xStartPosRight, yStartPosBottom, 0);
			glScalef(fontScaleMediumX, fontScaleMediumY, 1.0);
			m_PolygonFont->Render("Search Information");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glColor4f(1.0, 1.0, 1.0, 0.5);
			glTranslatef(xStartPosRight, yStartPosBottom - yOffsetMedium, 0);
			glScalef(fontScaleSmallX, fontScaleSmallY, 1.0);
			m_PolygonFont->Render("Right ascension: 180.00 deg");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(xStartPosRight, yStartPosBottom - 2*yOffsetMedium, 0);
			glScalef(fontScaleSmallX, fontScaleSmallY, 1.0);
			m_PolygonFont->Render("Declination: 45.00 deg");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(xStartPosRight, yStartPosBottom - 3*yOffsetMedium, 0);
			glScalef(fontScaleSmallX, fontScaleSmallY, 1.0);
			m_PolygonFont->Render("Frequency: 850.05 Hz");
			glPopMatrix();
	
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(xStartPosRight, yStartPosBottom - 4*yOffsetMedium, 0);
			glScalef(fontScaleSmallX, fontScaleSmallY, 1.0);
			m_PolygonFont->Render("Spindown rate: 3.25 * 10^-10 Hz/s");
			glPopMatrix();
		}
	
		// restore original state
		glMatrixMode( GL_PROJECTION);
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW);
		glPopMatrix();
		
		// disable FSAA
		glDisable(GL_MULTISAMPLE_ARB);
	}

	glFlush();
	glFinish();

	SDL_GL_SwapBuffers();
}

/**
 * View control
 */
void Starsphere::rotateSphere(const int relativeRotation,
        const int relativeElevation)
{
	// elevation
	viewpt_elev += relativeElevation/10.0;
	if (viewpt_elev > 89.0)
		viewpt_elev = +89.0;
	if (viewpt_elev < -89.0)
		viewpt_elev = -89.0;

	// rotation
	rotation_offset -= relativeRotation/10.0;
}

void Starsphere::zoomSphere(const int relativeZoom)
{
	// zoom
	viewpt_radius -= relativeZoom/10.0;
	if (viewpt_radius > 15.0)
		viewpt_radius = 15.0;
	if (viewpt_radius < 0.5)
		viewpt_radius = 0.5;
}

/**
 * Feature control
 */
void Starsphere::setFeature(const int feature, const bool enable)
{
	featureFlags = enable ? (featureFlags | feature) : (featureFlags & ~feature);
}

bool Starsphere::isFeature(const int feature)
{
	return ((featureFlags & feature) == feature ? true : false);
}
