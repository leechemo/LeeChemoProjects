package Domain;

import DAL.OrderDTO;

import java.time.LocalDate;
import java.util.*;

public class OperationsController {
//    private SiteRepository siteRepo;
    private BranchesRepository branchRepo;
    private OrderRepository orderRepo;
    private ItemRepository itemRepo;

    public OperationsController()
    {
        branchRepo = new BranchesRepository();
        orderRepo = new OrderRepository();
        itemRepo = new ItemRepository();
    }

    //**************************************SITE:**************************************//
//    public boolean addSite(Dictionary<String, String> data) {
//        int siteID = Integer.parseInt(data.get("branchNum"));
//        if (searchSite(siteID)) {
//            return false;
//        }
//        String address = data.get("address");
//        String zone = data.get("zone");
//        String contactName = data.get("contactName");
//        String phoneNumber = data.get("phoneNumber");
//        Site newSite = new Site(address, zone, contactName, phoneNumber, siteID);
//        return siteRepo.insert(newSite);
//    }
//    public boolean removeSite(Dictionary<String,String> data )
//    {
//        int idSite = Integer.parseInt(data.get("id"));
//        Site s = getSite(idSite);
//        return removeSite(idSite);
//    }
//
//    public boolean removeSite(int id) {
//        if (!searchSite(id))
//            return false;
//        return siteRepo.remove(id);
//    }
//    public boolean searchSite(int idSite){return siteRepo.search(idSite);}
//
//    public Site getSite(Dictionary<String, String> data)
//    {
//        int siteID = Integer.parseInt(data.get("id"));
//        return getSite(siteID);
//    }
//
//    public Site getSite(int id){
//        if (!searchSite(id)){
//            return null;
//        }
//        return siteRepo.get(id);
//    }
//
//    public SiteRepository getSiteRepo() {return siteRepo;}
//
//    public int sizeOfSites(){return siteRepo.getSizeSites();}
//
//
//    public String toString(String zone) {
//        StringBuilder result = new StringBuilder();
//        for (Site site : siteRepo.getSites()) {
//            if(site.getSiteZone().compareTo(zone)==0)
//                result.append(site.toString()).append("\n");
//        }
//        return result.toString();
//    }
//    public boolean validMatchZone(String source, String destination ) {return siteRepo.validMatchZone(source,destination);}
    //**************************************ORDER:**************************************//

    public int addOrder(Dictionary<String,String> data1,  Dictionary<Integer, ArrayList<String>> data2) {
        LocalDate date = LocalDate.of(Integer.parseInt(data1.get("year")),Integer.parseInt(data1.get("month")),Integer.parseInt(data1.get("day")));
        int destID = Integer.parseInt(data1.get("destination"));
        int sourceID = Integer.parseInt(data1.get("source"));
        Branch destination = branchRepo.getBranch(destID);
        Branch source = branchRepo.getBranch(sourceID);

        ArrayList<Item> orderItems = new ArrayList<>() ;
        for (Map.Entry<Integer, ArrayList<String>> key : ((Hashtable<Integer, ArrayList<String>>) data2).entrySet())
        {
            ArrayList<String> itemData = key.getValue();
            Item item = addItem(itemData); // create item from data
            orderItems.add(item); // add new item from data to list
        }
        int maxID = orderRepo.getMaxId() + 1;
        Order newOrder = new Order(maxID,date,destID,destination,sourceID,source,orderItems);
        orderRepo.insert(newOrder);

        for (Item item: orderItems) {
            item.setIdO(newOrder.getId());
            itemRepo.insert(item);
        }

        return maxID;
    }
    public boolean removeOrder(Dictionary<String, String> data)
    {
        int orderID = Integer.parseInt(data.get("id"));
        return remove(orderID);
    }
    public boolean remove(int id) {
        if (!searchOrder(id))
            return false;
        return orderRepo.remove(id);
    }
    public boolean searchOrder(int id) {return orderRepo.search(id);}

    public OrderRepository getOrderRepo() {return orderRepo;}

    public List<Integer> getOrderIdsByTransportId(int idTransport){return orderRepo.getOrderIdsByTransportId(idTransport);}

    public Order getOrder(int id) {
        if(!searchOrder(id))
        {
            return null;
        }
        Order order = orderRepo.get(id);
        Branch source = branchRepo.getBranch(order.getSourceID());
        Branch destination = branchRepo.getBranch(order.getDestinationID());
        order.setSource(source);
        order.setDestination(destination);
        List<Integer> items = itemRepo.getItemsByOrderId(id);
        ArrayList<Item> itemsOfOrder = new ArrayList<>();
        if(!items.isEmpty())
        {
            for (Integer itemID: items)
            {
                Item item = itemRepo.get(itemID);
                itemsOfOrder.add(item);
            }
            order.setItems(itemsOfOrder);
        }
        return order;
    }
    public boolean changeDestination(Dictionary<String, String> data) {
        int idOrder = Integer.parseInt(data.get("id"));
        int idSite = Integer.parseInt(data.get("idS"));
        Branch site = branchRepo.getBranch(idSite);
        Order orderTemp = getOrder(idOrder);
        if(site.getZone().compareTo(orderTemp.getDestination().getZone()) != 0) {
            return false;
        }
        orderRepo.updateSiteDest(orderTemp, site);
        return true;
    }
    ////to String for order
    public String generateOrderReport(String orderId) {

        Order order = getOrder(Integer.parseInt(orderId));
        StringBuilder sb = new StringBuilder();
        sb.append(order.toStringReport());
        return sb.toString();
    }

    public String printOrder(Dictionary<String, String> data)
    {
        int orderId = Integer.parseInt(data.get("orderID"));
        Order getOrder = getOrder(orderId);
        StringBuilder sb = new StringBuilder();
        sb.append(getOrder.toStringReport());
        return sb.toString();
    }

    public int getSizeOrders() {return orderRepo.getSizeOrders();}

    //**************************************ITEM:**************************************//
    public Item addItem(ArrayList<String> item) {
        int id = Integer.parseInt(item.get(0));
        String name = item.get(1);
        int amount = Integer.parseInt(item.get(2));
        Item newItem = new Item(id, name, amount);
        return newItem;
    }
    public boolean searchItem(int id) {return itemRepo.search(id);}

    public boolean associatedTransport(int id) {
        Order o = getOrder(id);
        return o.isHaveTransport();
    }
    public Item getItem(int itemID) {
        if(!searchItem(itemID))
        {
            return null;
        }
        return itemRepo.get(itemID);
    }

    public void updateIDTransport(int idOrder,int idTransport){orderRepo.updateIDTransport(idOrder,idTransport);}

    public ItemRepository getItemRepo() {return itemRepo;}
}
