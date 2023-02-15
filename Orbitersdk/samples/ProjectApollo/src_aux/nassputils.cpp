/***************************************************************************
  This file is part of Project Apollo - NASSP

  NASSP utility functions

  Project Apollo is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Project Apollo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Project Apollo; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://nassp.sourceforge.net/license/ for more details.

  **************************************************************************/

#include "nassputils.h"
#include <stdarg.h>
#include <algorithm>

namespace nassp
{
	namespace utils
	{

		void skpTextFmt(oapi::Sketchpad* skp, int x, int y, const char* fmt, ...) {
			char buf[512];
			va_list ap;
			va_start(ap, fmt);
			int len = vsnprintf(buf, sizeof(buf), fmt, ap);
			va_end(ap);
			len = (std::min)(len, (int)sizeof(buf) - 1);
			buf[sizeof(buf) - 1] = '\0';
			skp->Text(x, y, buf, len);
		}

		char* fmt(char* buf, int len, const char* format, ...) {
			va_list ap;
			va_start(ap, format);
			vsnprintf(buf, len, format, ap);
			va_end(ap);
			buf[len - 1] = '\0';
			return buf;
		}

	}
}
