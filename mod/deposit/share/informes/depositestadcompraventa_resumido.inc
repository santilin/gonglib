REPORT EstadCompraVenta_Inc
{
	Units = chars;
	SizeX = 80;
	SizeY = 60;
	Tag = "Estadísticas";

#ifdef INCLIQUIDACION
	PageOrientation = landscape;
	SizeX = 90;
#endif

	Include = "estadcompraventa_resumido_comun.inc";

	INPUT default {
		driver = mysql;
#ifdef INCLIQUIDACION
		INPUTFIELD VT_IMPORTELIQUIDACION {
			SOURCE = VT.IMPORTELIQUIDACION;
			Type = double;
		}
		INPUTFIELD VT_IMPORTELIQUIDACIONIVA {
			SOURCE = VT.IMPORTELIQUIDACIONIVA;
			Type = double;
		}
#endif
	}

	PAGEHEADER header_captions {
#ifdef INCLIQUIDACION
		OBJECT LIQUIDACION {
			value = "Liq. prov."; // "= const @DDD.ALBARANVENTADET_IMPORTELIQUIDACION_CAPTION;";
			posx = -12;
			sizex = 12;
			alignment = right;
		}
#endif
	}


#ifdef AGRUPAR
	GROUPFOOTER grupo_footer {
#ifdef INCLIQUIDACION
		OBJECT IMPORTELIQUIDACION {
#ifdef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTELIQUIDACIONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTELIQUIDACION";
#endif
			posx = -10;
			aggregate = sum;
			sizex = 10;
			alignment = right;
			format="%$";
		}
#endif // INCLIQUIDACION
	} // group footer
#endif // AGRUPAR


	REPORTFOOTER report_footer
	{
#ifdef INCLIQUIDACION
		OBJECT CALC_IMPORTELIQUIDACION {
#ifdef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTELIQUIDACIONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTELIQUIDACION";
#endif
			posx = -10;
			aggregate = sum;
			sizex = 10;
			alignment = right;
			format="%$";
		}
#endif // INCLIQUIDACION
	}
