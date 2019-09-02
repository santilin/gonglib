REPORT DepositEstadCompraVenta_Inc
{
	Units = chars;
	SizeX = 95;
	SizeY = 60;
	Tag = "Estadísticas";

#ifndef AGRUPAR
	PageOrientation = landscape;
	SizeX = 120;
#endif
#ifdef INCLIQUIDACION
	PageOrientation = landscape;
	SizeX = 130;
#ifdef AGRUPAR
	PageOrientation = landscape;
	SizeX = 90;
#endif
#endif

	Include = "estadcompraventa_comun.inc";

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
#ifndef AGRUPAR
	PAGEHEADER header_captions {
#endif
#ifdef AGRUPAR
	GROUPHEADER header_captions {
#endif
#ifdef INCLIQUIDACION
		OBJECT LIQUIDACION {
			value = "Liq. prov."; // "= const @DDD.ALBARANVENTADET_IMPORTELIQUIDACION_CAPTION;";
			posx = -12;
			sizex = 12;
			alignment = right;
		}
#endif
	}

	DETAILS details {
#ifdef INCLIQUIDACION
		OBJECT CALC_LIQUIDACION {
#ifdef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTELIQUIDACIONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTELIQUIDACION";
#endif
			posx = -10;
			sizex = 10;
			alignment = right;
			format="%$";
		}
#endif // INCLIQUIDACION
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
		OBJECT articulo_liquidacion {
			value = "Ganancia: ";
			posx = -30;
			sizex = 10;
		}
		OBJECT CALC_LIQTOTAL {
#ifdef PRECIOS_CON_IVA
			value = "= @input.VT_IMPORTECONIVA - @input.VT_IMPORTELIQUIDACIONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "= @input.VT_IMPORTE - @input.VT_IMPORTELIQUIDACION";
#endif
			posx = -1;
			aggregate = sum;
			sizex = 9;
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
		OBJECT articulo_liquidacion {
			value = "Ganancia: ";
			posx = -30;
			sizex = 6;
		}
		OBJECT CALC_LIQTOTAL {
#ifdef PRECIOS_CON_IVA
			value = "= @input.VT_IMPORTECONIVA - @input.VT_IMPORTELIQUIDACIONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "= @input.VT_IMPORTE - @input.VT_IMPORTELIQUIDACION";
#endif
			posx = -1;
			aggregate = sum;
			sizex = 9;
			alignment = right;
			format="%$";
		}
#endif // INCLIQUIDACION
	}
