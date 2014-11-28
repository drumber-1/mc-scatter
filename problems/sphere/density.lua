function get_density(x, y, z)
	r2 = x*x + y*y + z*z
	if r2 < 1 then
		return 5.0
	else
		return 0.0001
	end
end
