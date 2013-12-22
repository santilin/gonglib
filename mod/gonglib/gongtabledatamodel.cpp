#include "gongtabledatamodel.h"

/**
	@class gong::TableDataModel

	Este modelo de datos representa una abstracción para proporcionar datos en forma tabular para ser mostrados/editados de forma gráfica. El modelo requiere una serie de vistas (dbViewDefinition) y una conexión a la base de datos. Cuando se establece una vista con setView(), el modelo recupera los datos de la base de datos segn la vista seleccionada.

	La clase DataTable accede a los datos del modelo mediante su posición (fila y columna). Sin embargo, las vistas definen sus datos mediante los nombres de los campos, por lo tanto, es necesario traducir las columnas a nombres de campos. Para ello, en setView(), el modelo debe rellenar el diccionario mFieldPos[fldname, pos], que haría posible la utilización de las funciones getFieldPos(const Xtring &fldname) y getColName(unsigned int col).

	El primer campo del modelo debe ser siempre el  de la fila y se obtiene con getRowID().

	El modelo puede opcionalmante tener un filtro, aunque cada una de las vistas puede tener también su propio filtro. Ambos filtros son combinados.

	Clases derivadas de esta son: dbRecordDataModel, dbRecordListDataModel
 */


namespace gong {

TableDataModel::TableDataModel(const dbViewDefinitionsList &views, const Xtring &filter)
    : mViewDefinitionsList(views), mFilter(filter),
      mColCount(0), mReadOnly(true), mHasFetchedAllRows(false), mGroupCount(false),
      mCurrentViewIndex(-1)
{
    if( !mFilter.isEmpty() ) {
        _GONG_DEBUG_PRINT(3, "Setting filter to datamodel: " + mFilter );
    }
}

int TableDataModel::findFieldPos(const Xtring& fldname) const
{
    if( mFieldPos.find( fldname ) != mFieldPos.end() )
        return mFieldPos[fldname];
    else
        return -1;
}


dbViewDefinition *TableDataModel::getView(int index) const
{
    if( getViewCount() == 0 ) {
        _GONG_DEBUG_WARNING( "No hay vistas definidas en este datamodel" );
        return 0;
    }
    if( index < (int) getViewCount() && index != -1 )
        return mViewDefinitionsList[index];
    else {
        _GONG_DEBUG_WARNING( Xtring::printf("No existe la vista %d en este datamodel", index) );
        return 0;
    }
}

void TableDataModel::addFilter(const Xtring &aFilter)
{
    if( aFilter.trim().isEmpty() )
        return;
    if( mFilter.isEmpty() )
        mFilter = aFilter;
    else
        mFilter = "(" + mFilter + ")AND(" + aFilter + ")";
}

void TableDataModel::setFilter(const Xtring &aFilter)
{
    mFilter = aFilter;
}

void TableDataModel::removeFilters()
{
    mFilter.clear();
}

void TableDataModel::addView( const dbViewDefinition *view )
{
    /// TODO: why the const_cast?
    mViewDefinitionsList.insert( view->getName(), const_cast<dbViewDefinition *>(view) );
}

void TableDataModel::deleteView( const dbViewDefinition *view )
{
    for( dbViewDefinitionsList::iterator it = mViewDefinitionsList.begin();
            it != mViewDefinitionsList.end(); ++it )
        if( (*it).second == view ) {
            delete (*it).second;
            mViewDefinitionsList.erase(it);
            return;
        }
}

const Xtring TableDataModel::getFilter(const Xtring& where, const Xtring& addTo, const Xtring& condsSep) const
{
    Xtring ret = mFilter;
    if( !addTo.isEmpty() && mFilter != addTo.trim() ) {
        _GONG_DEBUG_PRINT(3, "Adding datamodel filter '" + mFilter + "' to '" + addTo + "'");
        if( !ret.isEmpty() )
            ret = "(" + ret + ")" + condsSep + "(" + addTo + ")";
        else
            ret = addTo;
    }
    if( !where.isEmpty() ) {
        if( !ret.isEmpty() )
            ret = " " + where + " (" + ret + ")";
        _GONG_DEBUG_PRINT(3, "+ filtro encadenado: " + ret );
    }
    return ret;
}

};
