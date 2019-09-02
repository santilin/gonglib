/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de semanas de pedidos
// TYPE dbRecord ventasweb::SemanaPedido
/*>>>>>MODULE_INFO*/

/*<<<<<SEMANAPEDIDO_INCLUDES*/
#include "ventaswebrecsemanapedido.h"
/*>>>>>SEMANAPEDIDO_INCLUDES*/

namespace gong {
namespace ventasweb {

#if 0
public function getFormattedDeliveryDate($format = "dd/MM/yyyy" )
{
    return Yii::app()->dateFormatter->format($format, $this->order_delivery_date);
}

public function lateOrderWeek($time)
{
    $lastorderweek = $this->find("order_start_time <= :p1 AND order_end_time >= :p1",
                                 array(":p1" => Utils::dateTimeToSql($time) ) );
    if( !$lastorderweek ) {
        $date = Utils::dateTimeToSql($time);
        $date = strtotime(date("Y-m-d H:i", strtotime($date)) . " -1 day");
        $lastorderweek = $this->find("order_start_time <= :p1 AND order_end_time >= :p1",
                                     array(":p1" => Utils::dateTimeToSql($date) ) );
    }
    return $lastorderweek;
}

public static function lastOrderWeek()
{
    $ow = OrderWeek::currentOrderWeek();
    if( !$ow )
        return OrderWeek::model()->find(array("order"=>"order_end_time desc", "limit" => 1) );
    else
        return $ow;
}

public static function currentOrderWeek()
{
    return OrderWeek::findByDate(time());
}

public static function findByDate($time)
{
    if( ! $time )
        die("OrderWeek::findByDate called with an empty date");
    return OrderWeek::model()->find("order_start_time <= :p1 AND order_end_time >= :p1",
                                    array(":p1" => Utils::dateTimeToSql($time) ) );
}

public function deliveryDateForShippingPoint($shipping_point_id)
{
    $shipping_point = ShippingPoint::model()->findByPk($shipping_point_id);
    return Utils::dateToSql(strtotime($this->order_delivery_date . " + " . $shipping_point->delivery_offset . " days"));
}
#endif

} // namespace ventasweb
} // namespace gong
