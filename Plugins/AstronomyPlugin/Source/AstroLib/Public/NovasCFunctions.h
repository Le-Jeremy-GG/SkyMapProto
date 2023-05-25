// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#if defined(_WIN32)
	#define ASTRODLLEXPORT __declspec(dllexport) 

#endif
#if defined(__MACH__)
	#define ASTRODLLEXPORT 
#endif

#define SIZE_OF_OBJ_NAME 51
#define SIZE_OF_CAT_NAME 15

//use 0 for full accuracy, 1 for reduced accuracy
const short int accuracy = 0;




extern "C"
{


	ASTRODLLEXPORT short int solarsystem(double tjd, short int body, short int origin, double *position, double *velocity);

	ASTRODLLEXPORT short int solarsystem_hp(double tjd[2], short body, short origin, double *position, double *velocity);


	/*
	Structures
	*/

	/*
	struct cat_entry:  basic astrometric data for any celestial object
	located outside the solar system; the catalog
	data for a star

	starname[SIZE_OF_OBJ_NAME] = name of celestial object
	catalog[SIZE_OF_CAT_NAME]  = catalog designator (e.g., HIP)
	starnumber                 = integer identifier assigned to object
	ra                         = ICRS right ascension (hours)
	dec                        = ICRS declination (degrees)
	promora                    = ICRS proper motion in right ascension
	(milliarcseconds/year)
	promodec                   = ICRS proper motion in declination
	(milliarcseconds/year)
	parallax                   = parallax (milliarcseconds)
	radialvelocity             = radial velocity (km/s)

	SIZE_OF_OBJ_NAME and SIZE_OF_CAT_NAME are defined below.  Each is the
	number of characters in the string (the string length) plus the null
	terminator.
	*/

	ASTRODLLEXPORT
	typedef struct
	{
		char starname[SIZE_OF_OBJ_NAME];
		char catalog[SIZE_OF_CAT_NAME];
		long int starnumber;
		double ra;
		double dec;
		double promora;
		double promodec;
		double parallax;
		double radialvelocity;
	} cat_entry;

	/*
	struct object:    specifies the celestial object of interest

	type              = type of object
	= 0 ... major planet, Pluto, Sun, or Moon
	= 1 ... minor planet
	= 2 ... object located outside the solar system
	(star, nebula, galaxy, etc.)
	number            = object number
	For 'type' = 0: Mercury = 1, ..., Pluto = 9,
	Sun = 10, Moon = 11
	For 'type' = 1: minor planet number
	For 'type' = 2: set to 0 (object is
	fully specified in 'struct cat_entry')
	name              = name of the object (limited to
	(SIZE_OF_OBJ_NAME - 1) characters)
	star              = basic astrometric data for any celestial object
	located outside the solar system; the catalog
	data for a star
	*/
	ASTRODLLEXPORT
	typedef struct
	{
		short int type;
		short int number;
		char name[SIZE_OF_OBJ_NAME];
		cat_entry star;
	} object;

	/*
	struct on_surface: data for an observer's location on the surface of
	the Earth.  The atmospheric parameters are used
	only by the refraction function called from
	function 'equ2hor'. Additional parameters can be
	added to this structure if a more sophisticated
	refraction model is employed.

	latitude           = geodetic (ITRS) latitude; north positive (degrees)
	longitude          = geodetic (ITRS) longitude; east positive (degrees)
	height             = height of the observer (meters)
	temperature        = temperature (degrees Celsius)
	pressure           = atmospheric pressure (millibars)
	*/
	ASTRODLLEXPORT
	typedef struct
	{
		double latitude;
		double longitude;
		double height;
		double temperature;
		double pressure;
	} on_surface;

	/*
	struct in_space:   data for an observer's location on a near-Earth
	spacecraft

	sc_pos[3]          = geocentric position vector (x, y, z), components
	in km
	sc_vel[3]          = geocentric velocity vector (x_dot, y_dot,
	z_dot), components in km/s

	Both vectors with respect to true equator and
	equinox of date
	*/
	ASTRODLLEXPORT
	typedef struct
	{
		double sc_pos[3];
		double sc_vel[3];
	} in_space;

	/*
	struct observer:   data specifying the location of the observer

	where              = integer code specifying location of observer
	= 0: observer at geocenter
	= 1: observer on surface of earth
	= 2: observer on near-earth spacecraft
	on_surface         = structure containing data for an observer's
	location on the surface of the Earth (where = 1)
	near_earth         = data for an observer's location on a near-Earth
	spacecraft (where = 2)
	*/
	ASTRODLLEXPORT
	typedef struct
	{
		short int where;
		on_surface on_surf;
		in_space near_earth;
	} observer;

	/*
	struct sky_pos:    data specifying a celestial object's place on the
	sky; contains the output from function 'place'

	r_hat[3]           = unit vector toward object (dimensionless)
	ra                 = apparent, topocentric, or astrometric
	right ascension (hours)
	dec                = apparent, topocentric, or astrometric
	declination (degrees)
	dis                = true (geometric, Euclidian) distance to solar
	system body or 0.0 for star (AU)
	rv                 = radial velocity (km/s)
	*/
	ASTRODLLEXPORT
	typedef struct
	{
		double r_hat[3];
		double ra;
		double dec;
		double dis;
		double rv;
	} sky_pos;

	/*
	struct ra_of_cio:  right ascension of the Celestial Intermediate
	Origin (CIO) with respect to the GCRS

	jd_tdb             = TDB Julian date
	ra_cio             = right ascension of the CIO with respect
	to the GCRS (arcseconds)
	*/
	ASTRODLLEXPORT
	typedef struct
	{
		double jd_tdb;
		double ra_cio;
	} ra_of_cio;


	/*
	Define "origin" constants.
	*/

#define BARYC  0
#define HELIOC 1

	/*
	Function prototypes
	*/
	ASTRODLLEXPORT
	double *readeph(int mp, char *name, double jd,

		int *err);
	ASTRODLLEXPORT
	short int app_star(double jd_tt, cat_entry *star,
		short int accuracy,

		double *ra, double *dec);
	ASTRODLLEXPORT
	short int virtual_star(double jd_tt, cat_entry *star,
		short int accuracy,

		double *ra, double *dec);
	ASTRODLLEXPORT
	short int astro_star(double jd_tt, cat_entry *star,
		short int accuracy,

		double *ra, double *dec);
	ASTRODLLEXPORT
	short int app_planet(double jd_tt, object *ss_body,
		short int accuracy,

		double *ra, double *dec, double *dis);
	ASTRODLLEXPORT
	short int virtual_planet(double jd_tt, object *ss_body,
		short int accuracy,

		double *ra, double *dec, double *dis);
	ASTRODLLEXPORT
	short int astro_planet(double jd_tt, object *ss_body,
		short int accuracy,

		double *ra, double *dec, double *dis);
	ASTRODLLEXPORT
	short int topo_star(double jd_tt, double delta_t, cat_entry *star,
		on_surface *position, short int accuracy,

		double *ra, double *dec);
	ASTRODLLEXPORT
	short int local_star(double jd_tt, double delta_t, cat_entry *star,
		on_surface *position, short int accuracy,

		double *ra, double *dec);
	ASTRODLLEXPORT
	short int topo_planet(double jd_tt, object *ss_body, double delta_t,
		on_surface *position, short int accuracy,

		double *ra, double *dec, double *dis);
	ASTRODLLEXPORT
	short int local_planet(double jd_tt, object *ss_body,
		double delta_t, on_surface *position,
		short int accuracy,

		double *ra, double *dec, double *dis);
	ASTRODLLEXPORT
	short int mean_star(double jd_tt, double ra, double dec,
		short int accuracy,

		double *ira, double *idec);
	ASTRODLLEXPORT
	short int place(double jd_tt, object *cel_object,
		observer *location, double delta_t,
		short int coord_sys, short int accuracy,

		sky_pos *output);
	ASTRODLLEXPORT
	void equ2gal(double rai, double deci,

		double *glon, double *glat);
	ASTRODLLEXPORT
	short int equ2ecl(double jd_tt, short int coord_sys,
		short int accuracy, double ra, double dec,

		double *elon, double *elat);
	ASTRODLLEXPORT
	short int equ2ecl_vec(double jd_tt, short int coord_sys,
		short int accuracy, double *pos1,

		double *pos2);
	ASTRODLLEXPORT
	short int ecl2equ_vec(double jd_tt, short int coord_sys,
		short int accuracy, double *pos1,

		double *pos2);
	ASTRODLLEXPORT
	void equ2hor(double jd_ut1, double delta_t, short int accuracy,
		double xp, double yp, on_surface *location, double ra,
		double dec, short int ref_option,

		double *zd, double *az, double *rar, double *decr);
	ASTRODLLEXPORT
	short int gcrs2equ(double jd_tt, short int coord_sys,
		short int accuracy, double rag, double decg,

		double *ra, double *dec);
	ASTRODLLEXPORT
	short int sidereal_time(double jd_high, double jd_low,
		double delta_t, short int gst_type,
		short int method, short int accuracy,

		double *gst);
	ASTRODLLEXPORT
	double era(double jd_high, double jd_low);
	ASTRODLLEXPORT
	short int ter2cel(double jd_ut_high, double jd_ut_low,
		double delta_t, short int method,
		short int accuracy, short int option, double xp,
		double yp, double *vec1,

		double *vec2);
	ASTRODLLEXPORT
	short int cel2ter(double jd_ut_high, double jd_ut_low,
		double delta_t, short int method,
		short int accuracy, short int option,
		double xp, double yp, double *vec1,

		double *vec2);
	ASTRODLLEXPORT
	void spin(double angle, double *pos1,

		double *pos2);
	ASTRODLLEXPORT
	void wobble(double tjd, short int direction, double xp, double yp,
		double *pos1,

		double *pos2);
	ASTRODLLEXPORT
	void terra(on_surface *location, double st,

		double *pos, double *vel);
	ASTRODLLEXPORT
	void e_tilt(double jd_tdb, short int accuracy,

		double *mobl, double *tobl, double *ee, double *dpsi,
		double *deps);
	ASTRODLLEXPORT
	short int cel_pole(double tjd, short int type, double dpole1,
		double dpole2);
	ASTRODLLEXPORT
	double ee_ct(double jd_high, double jd_low, short int accuracy);
	ASTRODLLEXPORT
	void frame_tie(double *pos1, short int direction,

		double *pos2);
	ASTRODLLEXPORT
	void proper_motion(double jd_tdb1, double *pos, double *vel,
		double jd_tdb2,

		double *pos2);
	ASTRODLLEXPORT
	void bary2obs(double *pos, double *pos_obs,

		double *pos2, double *lighttime);
	ASTRODLLEXPORT
	short int geo_posvel(double jd_tt, double delta_t,
		short int accuracy, observer *obs,

		double *pos, double *vel);
	ASTRODLLEXPORT
	short int light_time(double jd_tdb, object *ss_object,
		double pos_obs[3], double tlight0,
		short int accuracy,

		double pos[3], double *tlight);
	ASTRODLLEXPORT
	double d_light(double *pos1, double *pos_obs);
	ASTRODLLEXPORT
	short int grav_def(double jd_tdb, short int loc_code,
		short int accuracy, double *pos1, double *pos_obs,

		double *pos2);
	ASTRODLLEXPORT
	void grav_vec(double *pos1, double *pos_obs, double *pos_body,
		double rmass,

		double *pos2);
	ASTRODLLEXPORT
	void aberration(double *pos, double *ve, double lighttime,

		double *pos2);
	ASTRODLLEXPORT
	void rad_vel(object *cel_object, double *pos, double *vel,
		double *vel_obs, double d_obs_geo, double d_obs_sun,
		double d_obj_sun,

		double *rv);
	ASTRODLLEXPORT
	short int precession(double jd_tdb1, double *pos1, double jd_tdb2,

		double *pos2);
	ASTRODLLEXPORT
	void nutation(double jd_tdb, short int direction, short int accuracy,
		double *pos,

		double *pos2);
	ASTRODLLEXPORT
	void nutation_angles(double t, short int accuracy,

		double *dpsi, double *deps);
	ASTRODLLEXPORT
	void fund_args(double t,

		double a[5]);
	ASTRODLLEXPORT
	double mean_obliq(double jd_tdb);
	ASTRODLLEXPORT
	short int vector2radec(double *pos,

		double *ra, double *dec);
	ASTRODLLEXPORT
	void radec2vector(double ra, double dec, double dist,

		double *vector);
	ASTRODLLEXPORT
	void starvectors(cat_entry *star,

		double *pos, double *vel);
	ASTRODLLEXPORT
	void tdb2tt(double tdb_jd,

		double *tt_jd, double *secdiff);
	ASTRODLLEXPORT
	short int cio_ra(double jd_tt, short int accuracy,

		double *ra_cio);
	ASTRODLLEXPORT
	short int cio_location(double jd_tdb, short int accuracy,

		double *ra_cio, short int *ref_sys);
	ASTRODLLEXPORT
	short int cio_basis(double jd_tdb, double ra_cio, short int ref_sys,
		short int accuracy,

		double *x, double *y, double *z);
	ASTRODLLEXPORT
	short int cio_array(double jd_tdb, long int n_pts,

		ra_of_cio *cio);
	ASTRODLLEXPORT
	double ira_equinox(double jd_tdb, short int equinox,
		short int accuracy);
	ASTRODLLEXPORT
	short int ephemeris(double jd[2], object *cel_obj, short int origin,
		short int accuracy,

		double *pos, double *vel);
	ASTRODLLEXPORT
	void transform_hip(cat_entry *hipparcos,

		cat_entry *hip_2000);
	ASTRODLLEXPORT
	short int transform_cat(short int option, double date_incat,
		cat_entry *incat, double date_newcat,
		char newcat_id[SIZE_OF_CAT_NAME],

		cat_entry *newcat);
	ASTRODLLEXPORT
	void limb_angle(double pos_obj[3], double pos_obs[3],

		double *limb_ang, double *nadir_ang);
	ASTRODLLEXPORT
	double refract(on_surface *location, short int ref_option,
		double zd_obs);
	ASTRODLLEXPORT
	double julian_date(short int year, short int month, short int day,
		double hour);
	ASTRODLLEXPORT
	void cal_date(double tjd,

		short int *year, short int *month, short int *day,
		double *hour);
	ASTRODLLEXPORT
	double norm_ang(double angle);
	ASTRODLLEXPORT
	short int make_cat_entry(char star_name[SIZE_OF_OBJ_NAME],
		char catalog[SIZE_OF_CAT_NAME],
		long int star_num, double ra, double dec,
		double pm_ra, double pm_dec, double parallax,
		double rad_vel,

		cat_entry *star);
	ASTRODLLEXPORT
	short int make_object(short int type, short int number,
		char name[SIZE_OF_OBJ_NAME],
		cat_entry *star_data,

		object *cel_obj);
	ASTRODLLEXPORT
	short int make_observer(short int where, on_surface *obs_surface,
		in_space *obs_space,

		observer *obs);
	ASTRODLLEXPORT
	void make_observer_at_geocenter(

		observer *obs_at_geocenter);
	ASTRODLLEXPORT
	void make_observer_on_surface(double latitude, double longitude,
		double height, double temperature,
		double pressure,

		observer *obs_on_surface);
	ASTRODLLEXPORT
	void make_observer_in_space(double sc_pos[3], double sc_vel[3],

		observer *obs_in_space);
	ASTRODLLEXPORT
	void make_on_surface(double latitude, double longitude,
		double height,
		double temperature, double pressure,

		on_surface *obs_surface);
	ASTRODLLEXPORT
	void make_in_space(double sc_pos[3], double sc_vel[3],

		in_space *obs_space);




	//----------------------------------------------------------------------
	/*
	ASTRODLLEXPORT double norm_ang(double angle);
	
	ASTRODLLEXPORT
		typedef struct
	{
		char starname[51];
		char catalog[4];
		long int starnumber;
		double ra;
		double dec;
		double promora;
		double promodec;
		double parallax;
		double radialvelocity;
	} cat_entry;

	ASTRODLLEXPORT 
	typedef struct
	{
		double latitude;
		double longitude;
		double height;
		double temperature;
		double pressure;
	} on_surface;


	
	ASTRODLLEXPORT
	typedef struct
	{
		double sc_pos[3];
		double sc_vel[3];
	} in_space;

	ASTRODLLEXPORT
	typedef struct
	{
		double r_hat[3];
		double ra;
		double dec;
		double dis;
		double rv;
	} sky_pos;

	
	ASTRODLLEXPORT
	typedef struct
	{
		short int type;
		short int number;
		char name[SIZE_OF_OBJ_NAME];
		cat_entry star;
	} object;


	ASTRODLLEXPORT
		typedef struct
	{
		short int where;
		on_surface on_surf;
		in_space near_earth;
	} observer;



	ASTRODLLEXPORT short int make_cat_entry(char star_name[51], char catalog[4], long int star_num, double ra, double dec, double pm_ra, double pm_dec, double parallax, double rad_vel, cat_entry *star);
	ASTRODLLEXPORT short int app_star(double, cat_entry*, short int, double*, double*);
	ASTRODLLEXPORT double julian_date(short int, short int, short int, double);
	ASTRODLLEXPORT short int topo_star(double jd_tt, double delta_t, cat_entry *star, on_surface *position, short int accuracy,double *ra, double *dec);
	ASTRODLLEXPORT void make_on_surface(double latitude, double longitude, double height,double temperature, double pressure,on_surface *obs_surface);
	ASTRODLLEXPORT short int place(double jd_tt, object *cel_object, observer *location, double delta_t, short int coord_sys, short int accuracy, sky_pos *output);
	*/
	}