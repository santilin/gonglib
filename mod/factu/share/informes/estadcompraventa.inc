REPORT EstadCompraVenta_Inc
{
	Units = chars;
	Tag = "Estadísticas";
	PageOrientation = landscape;
	SizeX = 120;
	SizeY = 48;
	
#ifdef AGRUPAR_POR_FECHA_DIARIO	
	SizeX = 120;
	SizeY = 50;
	PageOrientation = landscape;
#endif	

#ifdef AGRUPAR_POR_ARTICULO
	SizeX = 120;
	PageOrientation = landscape;
	SizeY = 50;
#endif	

#ifdef AGRUPAR_POR_DOCUMENTO
	PageOrientation = portrait;
	SizeX = 100;
	SizeY = 60;
#endif	
	Include = "estadcompraventa_comun.inc";