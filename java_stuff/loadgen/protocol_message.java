import java.util.HashMap;
import java.util.Map;
import java.util.Iterator;


public class protocol_message {
	private HashMap dataMap;
	public protocol_message(String message_type){
		dataMap = new HashMap();
		dataMap.put("TYPE",message_type);
		}
	public void add_field(String name, String value){
		dataMap.put(name,value);
		}
	public String serialize_message(){
		Iterator it = dataMap.entrySet().iterator();
		String retVal = "";
		while(it.hasNext()){
			Map.Entry entry =(Map.Entry)it.next();
			retVal=entry.getKey() + "=" + entry.getValue() + "|" + retVal;
			}
		return retVal;
		}
}

