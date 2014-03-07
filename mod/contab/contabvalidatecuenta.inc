	if( sender == editCuentaCuenta && !editCuentaCuenta->toString().trim().isEmpty() ) {
		contab::Cuenta cuenta( editCuentaCuenta->toString(), contab::ModuleInstance->getDigitosTrabajo() );
		cuenta.expandir();
		dbRecordID recid = 0;
		Xtring validmsg;
		if( cuenta.isValid(validmsg) && cuenta.isGrupo() ) {
			// Mostrar cuentas del grupo
			Xtring where = getRecord()->getConnection()->toSQLStartLike("cuenta", cuenta);
			dbViewDefinitionDict viewsforcuenta;
			DBAPP->getDatabase()->getViewsForTable(getRecCuenta()->getTableName(), viewsforcuenta);
			recid = DBAPP->choose(this, getRecCuenta(),
				new dbRecordDataModel(getRecCuenta(), viewsforcuenta, where),
		/// \todo {autodelete} http://code.axter.com/cow_ptr.h
				dbApplication::editNone, this);
			if( recid == 0 ) { // No mostrar mensaje
				*isvalid = false;
			}
		} else {
			recid = DBAPP->seekCode(getRecCuenta(), this,
					"cuenta", cuenta, "descripcion");
		}
		if( recid != 0 ) {
			cuenta = contab::Cuenta( getRecCuenta()->getValue("CUENTA").toString(), contab::ModuleInstance->getDigitosTrabajo() );
			if( cuenta.isGrupo() ) {
				validresult->addError( Xtring::printf("La cuenta '%s' es un grupo. Elija una cuenta de diario, por ejemplo '%s.1'",
					cuenta.c_str(), cuenta.c_str() ), "CUENTA" );
				*isvalid = false;
			} else {
				scatterCuenta();
			}
		} else {
			validresult->addError( Xtring::printf("La cuenta '%s' no existe", cuenta.c_str() ), "CUENTA");
			*isvalid = false;
		}
	}
