/*
range_lon = 180
range_lat = 85.05112877980659
def lon2tile(lon, n):
	return int((range_lon+lon) * n / range_lon/2)
def lat2tile(lat, n):
    lat_rad = math.radians(lat)
    ytile = int((1.0 - math.asinh(math.tan(lat_rad)) / math.pi) / 2.0 * n)
    return ytile
*/