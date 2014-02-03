import org.web3d.vrml.lang.VRMLNode;
import org.web3d.vrml.nodes.VRMLFieldData;

public class X3DParserTest {
	X3DParser parser = new X3DParser();
	
	// ----------------------------------------------
	// ----------------------------------------------
	public void test0(String fname)
	{
		parser.parse(fname);
		
		VRMLNode[] rootChildren = parser.getChildrenOfRootNode();
		for (int i=0; i<rootChildren.length; i++)
		{
			printNode(rootChildren[i], 0);
		}
	}
	
	// ----------------------------------------------
	//
	// ----------------------------------------------
	public void test1(String fname)
	{
		parser.parse(fname);
		
		String[] defNames = parser.getDefNames();
		for (int i=0; i<defNames.length; i++)
		{
			String defName = defNames[i];
			System.out.println(defName);
		}
	}
	
	// ----------------------------------------------
	//
	// ----------------------------------------------
	public void test2(String fname, String defName)
	{
		parser.parse(fname);

		VRMLNode node = parser.getDefNode(defName);
		
		if (node != null)
		{
			System.out.println("***  "+defName+"  ***");
			printNode(node, 0);
		}
	}
	
	// ----------------------------------------------
	// ----------------------------------------------
	void printNode(VRMLNode node, int indent)
	{
		String indentStr = X3DParser.getIndentSpace(indent);
		System.out.print(indentStr);
		System.out.println(node.getVRMLNodeName());
		
		String[] fieldNames = parser.getFieldNames(node);
		
		for (int i=0; i<fieldNames.length; i++)
		{
			String fieldName = fieldNames[i];
			indentStr = X3DParser.getIndentSpace(indent+1);
			System.out.print(indentStr+"[ "+fieldName+" ] ");

			VRMLFieldData data = parser.getFieldValue(node, fieldName);
			
			if (parser.isSFNodeField(node, fieldName))
			{
				System.out.println("");
				
				VRMLNode child = data.nodeValue;
				if (child != null)
				{
					printNode(child, indent+2);
				}
			}
			else if (parser.isMFNodeField(node, fieldName))
			{
				System.out.println("");
				
				VRMLNode[] children = data.nodeArrayValue;
				for (int ind=0; ind<children.length; ind++)
				{
					VRMLNode child = children[ind];
					if (child != null)
					{
						printNode(child, indent+2);
					}
				}
			}
			else
			{
				String dataString = MyX3DUtil.VRMLFieldDataToString(data);
				System.out.println(dataString);
			}
		}
	}
	
	// ----------------------------------------------
	// ----------------------------------------------
	public static void main(String[] args)
	{
		X3DParserTest t = new X3DParserTest();

		if (args.length < 1)
		{
			System.err.println("usage: X3DParser test-number [args]");
			return;
		}
		
		int testNumber = Integer.parseInt(args[0]);
		
		switch (testNumber)
		{
		case 0:
			t.test0(args[1]);
			break;
			
		case 1:
			t.test1(args[1]);
			break;
			
		case 2:
			t.test2(args[1], args[2]);
			break;
		}
	}
}

