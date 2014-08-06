import org.web3d.vrml.lang.VRMLNode;
import org.web3d.vrml.nodes.VRMLFieldData;

public class MyX3DUtil {
	// -------------------------------------------------------------
	//
	// -------------------------------------------------------------
	public static String VRMLFieldDataToString(VRMLFieldData data)
	{
		String ret = "";

		try
		{
			switch (data.dataType)
			{
			// -----------------------------------
			// -----------------------------------
			case VRMLFieldData.BOOLEAN_DATA:
				ret = Boolean.toString(data.booleanValue);
				break;

			case VRMLFieldData.INT_DATA:
				ret = Integer.toString(data.intValue);
				break;

			case VRMLFieldData.LONG_DATA:
				ret = Long.toString(data.longValue);
				break;

			case VRMLFieldData.FLOAT_DATA:
				ret = Float.toString(data.floatValue);
				break;

			case VRMLFieldData.DOUBLE_DATA:
				ret = Double.toString(data.doubleValue);
				break;

			case VRMLFieldData.STRING_DATA:
				ret = data.stringValue;
				break;

			case VRMLFieldData.NODE_DATA:
				VRMLNode node = data.nodeValue;
				ret = "[SFNode] ";
				if (node != null)
				{
					ret = ret + node.getVRMLNodeName();
				}
				break;

				// -----------------------------------
				// -----------------------------------
			case VRMLFieldData.BOOLEAN_ARRAY_DATA:
			{
				boolean[] a = data.booleanArrayValue;
				for (int i=0; i<a.length; i++)
				{
					ret = ret + Boolean.toString(a[i]) + " ";
				}
			}
			break;

			case VRMLFieldData.INT_ARRAY_DATA:
			{
				int[] a = data.intArrayValue;
				for (int i=0; i<a.length; i++)
				{
					ret = ret + Integer.toString(a[i]) + " ";
				}
			}
			break;

			case VRMLFieldData.LONG_ARRAY_DATA:
			{
				long[] a = data.longArrayValue;
				for (int i=0; i<a.length; i++)
				{
					ret = ret + Long.toString(a[i])+" ";
				}
			}
			break;

			case VRMLFieldData.FLOAT_ARRAY_DATA:
			{
				float[] a = data.floatArrayValue;
				for (int i=0; i<a.length; i++)
				{
					ret = ret + Float.toString(a[i]) + " ";
				}
			}
			break;

			case VRMLFieldData.DOUBLE_ARRAY_DATA:
			{
				double[] a = data.doubleArrayValue;
				for (int i=0; i<a.length; i++)
				{
					ret = ret + Double.toString(a[i]) + " ";
				}
			}
			break;

			case VRMLFieldData.NODE_ARRAY_DATA:
			{
				VRMLNode[] a = data.nodeArrayValue;

				ret = "[MFNode] ";
				if (a != null)
				{
					int len = a.length;
					if (len > 0)
						ret = ret + a[0].getVRMLNodeName()+" ("+Integer.toString(len)+")";
					else 
						ret = ret + "null";
				}
				else
					ret = ret + "null";
			}
			break;

			case VRMLFieldData.STRING_ARRAY_DATA:
			{
				String[] a = data.stringArrayValue;
				for (int i=0; i<a.length; i++)
				{
					ret = ret + a[i].toString() + " ";
				}
			}
			break;
			}
		}
		catch (java.lang.NullPointerException excep)
		{
			// add try..catch by sekikawa (2009/3/9)
			// in parsing some .x3d file, java stopped by
			// NullPointerException.
			// so I added exception handler to avoid this error.
			// if NullPointerException occurred, this method returns "".
			ret = "";
		}

		return ret;
	}
}

