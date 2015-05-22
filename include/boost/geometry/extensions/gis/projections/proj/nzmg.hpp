#ifndef BOOST_GEOMETRY_PROJECTIONS_NZMG_HPP
#define BOOST_GEOMETRY_PROJECTIONS_NZMG_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_zpoly1.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace nzmg
    {

            static const double EPSLN = 1e-10;
            static const double SEC5_TO_RAD = 0.4848136811095359935899141023;
            static const double RAD_TO_SEC5 = 2.062648062470963551564733573;
            static const int Nbf = 5;
            static const int Ntpsi = 9;
            static const int Ntphi = 8;

                static COMPLEX
            bf[] = {
                {.7557853228,    0.0},
                {.249204646,    .003371507},
                {-.001541739,    .041058560},
                {-.10162907,    .01727609},
                {-.26623489,    -.36249218},
                {-.6870983,    -1.1651967} };
                static double
            tphi[] = { 1.5627014243, .5185406398, -.03333098, -.1052906, -.0368594,
                .007317, .01220, .00394, -.0013 },
            tpsi[] = { .6399175073, -.1358797613, .063294409, -.02526853, .0117879,
                -.0055161, .0026906, -.001333, .00067, -.00034 };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_nzmg_ellipsoid : public base_t_fi<base_nzmg_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;


                inline base_nzmg_ellipsoid(const Parameters& par)
                    : base_t_fi<base_nzmg_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    COMPLEX p;
                    double *C;
                    int i;

                    lp_lat = (lp_lat - this->m_par.phi0) * RAD_TO_SEC5;
                    for (p.r = *(C = tpsi + (i = Ntpsi)); i ; --i)
                        p.r = *--C + lp_lat * p.r;
                    p.r *= lp_lat;
                    p.i = lp_lon;
                    p = pj_zpoly1(p, bf, Nbf);
                    xy_x = p.i;
                    xy_y = p.r;
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    int nn, i;
                    COMPLEX p, f, fp, dp;
                    double den, *C;

                    p.r = xy_y;
                    p.i = xy_x;
                    for (nn = 20; nn ;--nn) {
                        f = pj_zpolyd1(p, bf, Nbf, &fp);
                        f.r -= xy_y;
                        f.i -= xy_x;
                        den = fp.r * fp.r + fp.i * fp.i;
                        p.r += dp.r = -(f.r * fp.r + f.i * fp.i) / den;
                        p.i += dp.i = -(f.i * fp.r - f.r * fp.i) / den;
                        if ((fabs(dp.r) + fabs(dp.i)) <= EPSLN)
                            break;
                    }
                    if (nn) {
                        lp_lon = p.i;
                        for (lp_lat = *(C = tphi + (i = Ntphi)); i ; --i)
                            lp_lat = *--C + p.r * lp_lat;
                        lp_lat = this->m_par.phi0 + p.r * lp_lat * SEC5_TO_RAD;
                    } else
                        lp_lon = lp_lat = HUGE_VAL;
                }
            };

            // New Zealand Map Grid
            template <typename Parameters>
            void setup_nzmg(Parameters& par)
            {
                /* force to International major axis */
                par.ra = 1. / (par.a = 6378388.0);
                par.lam0 = DEG_TO_RAD * 173.;
                par.phi0 = DEG_TO_RAD * -41.;
                par.x0 = 2510000.;
                par.y0 = 6023150.;
            }

        }} // namespace detail::nzmg
    #endif // doxygen

    /*!
        \brief New Zealand Map Grid projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Fixed Earth
        \par Example
        \image html ex_nzmg.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct nzmg_ellipsoid : public detail::nzmg::base_nzmg_ellipsoid<Geographic, Cartesian, Parameters>
    {
        inline nzmg_ellipsoid(const Parameters& par) : detail::nzmg::base_nzmg_ellipsoid<Geographic, Cartesian, Parameters>(par)
        {
            detail::nzmg::setup_nzmg(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class nzmg_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<nzmg_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void nzmg_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("nzmg", new nzmg_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_NZMG_HPP

