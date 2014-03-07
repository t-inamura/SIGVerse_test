import java.io.File;
import java.io.FileInputStream;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

//import org.ietf.uri.*;

import org.web3d.util.DefaultErrorReporter;
import org.web3d.vrml.lang.*;
import org.web3d.vrml.nodes.*;
import org.web3d.vrml.parser.VRMLParserFactory;
import org.web3d.vrml.renderer.CRVRMLScene;
import org.web3d.vrml.renderer.common.nodes.core.*;
import org.web3d.vrml.sav.InputSource;
import org.web3d.vrml.sav.VRMLReader;

public class X3DParser {
	X3DParseEventHandler handler;
	CRVRMLScene scene;
	
	// ----------------------------------------------------------
	//	parse
	// ----------------------------------------------------------
	public boolean parse(String fname)
	{
		File f = new File(fname);
		if (!f.exists())
		{
			System.err.println("*** [ERROR] "+fname+" not exists! ***");
			return false;
		}

		FileInputStream stream = null;

		try
		{
			stream = new FileInputStream(fname);
		}
		catch (Exception excep)
		{
			System.out.println(excep.toString());
			return false;
		}

		//	InputSource is = new InputSource(f);
		InputSource is = new InputSource("file://"+fname, stream);

		VRMLParserFactory parser_fac = null;

		try
		{
			parser_fac = VRMLParserFactory.newVRMLParserFactory();
		}
		catch(Exception fce)
		{
			throw new RuntimeException("Failed to load factory");
		}

		VRMLReader reader = parser_fac.newVRMLReader();

		scene = new CRVRMLScene(2, 0);
		handler = new X3DParseEventHandler(scene);
		
		handler.debugMode(false);
		//handler.debugMode(true);

		reader.setHeaderIgnore(true);
		reader.setContentHandler(handler);
		reader.setScriptHandler(handler);
		reader.setProtoHandler(handler);
		reader.setRouteHandler(handler);
		reader.setErrorReporter(DefaultErrorReporter.getDefaultReporter());

		try
		{
			reader.parse(is);
		}
		catch (Exception excep)
		{
			excep.printStackTrace();
			return false;
		}

//		VRMLNode root = scene.getRootNode();
//		printNode(root);
		
		return true;
	}

	public static String getIndentSpace(int indentLevel)
	{
		String indentStr = "";

		for (int i=0; i<indentLevel; i++)
		{
			indentStr = indentStr + "  ";
		}

		return indentStr;
	}

	void printNode(VRMLNode node)
	{
		printNodeSub(node, 0);
	}

	void printNodeSub(VRMLNode node, int indentLevel)
	{
		String indentStr = X3DParser.getIndentSpace(indentLevel);
		System.out.print(indentStr);
		System.out.println(node.getVRMLNodeName());

		if (node instanceof BaseWorldRoot)
		{

			BaseWorldRoot root = (BaseWorldRoot)node;

			VRMLNodeType[] childNodes = root.getChildren();
			for (int i=0; i<childNodes.length; i++)
			{
				VRMLNode child=childNodes[i];
				printNodeSub(child, indentLevel+1);
			}
		}
		else
		{
			int n = node.getNumFields();

//			System.out.print(indentStr);
//			System.out.println(Integer.toString(n)+" fields");

			for (int i=1; i<n; i++)
			{
//				System.out.print(indentStr);
//				System.out.print(i);

				VRMLFieldDeclaration decl = node.getFieldDeclaration(i);
				if (decl != null)
				{
					String fieldName = decl.getName();	// field name
//					System.out.println(" "+fieldName);

					int fieldAccess = decl.getAccessType();
					if ((fieldAccess == FieldConstants.FIELD) || (fieldAccess == FieldConstants.EXPOSEDFIELD))
					{
						printField(node, fieldName, indentLevel);
					}
				}
			}
		}
	}

	void printField(VRMLNode node, String fieldName, int indentLevel)
	{
		String indentStr;

		indentStr = X3DParser.getIndentSpace(indentLevel+1);
		System.out.print(indentStr);

		int i = node.getFieldIndex(fieldName);
		VRMLFieldData data = ((VRMLNodeType)node).getFieldValue(i);

		if (data.dataType == VRMLFieldData.NODE_DATA)
		{
			System.out.println("[ "+fieldName+" ]");

			VRMLNode child = data.nodeValue;
			if (child != null)
			{
				printNodeSub(child, indentLevel+2);
			}
		}
		else if (data.dataType == VRMLFieldData.NODE_ARRAY_DATA)
		{
			System.out.println("[ "+fieldName+" ]");

			VRMLNode[] children = data.nodeArrayValue;
			for (int ind=0; ind<children.length; ind++)
			{
				VRMLNode child = children[ind];
				if (child != null)
				{
					printNodeSub(child, indentLevel+2);
				}
			}
		}
		else
		{
			String dataString = MyX3DUtil.VRMLFieldDataToString(data);

			System.out.println("[ "+fieldName+" ] : "+dataString);
		}
	}

	// **********************************************************
	//  Interface methods for program using this parser
	// **********************************************************
	
	// ---------------------------------------------
	//  get root node of scene
	// ---------------------------------------------
	public VRMLNode getRootNode()
	{
		return (scene != null) ? scene.getRootNode() : null;
	}
	
	// ---------------------------------------------
	//  get all children of root node
	// ---------------------------------------------
	public VRMLNode[] getChildrenOfRootNode()
	{
		VRMLNode root = getRootNode();
		
		if (root instanceof BaseWorldRoot)
		{
			VRMLNodeType[] childNodes = ((BaseWorldRoot)root).getChildren();

			VRMLNode[] children = new VRMLNode[childNodes.length];
			for (int i=0; i<childNodes.length; i++)
			{
				children[i]=childNodes[i];
			}
			
			return children;
		}
		else
		{
			return null;
		}
	}
	
	// ---------------------------------------------
	//  get all field names belongs to node
	// ---------------------------------------------
	public String[] getFieldNames(VRMLNode node)
	{
		int i;
		int n = node.getNumFields();

//		System.out.print(indentStr);
//		System.out.println(Integer.toString(n)+" fields");

		int nValidFields = 0;
		for (i=1; i<n; i++)
		{
//			System.out.print(indentStr);
//			System.out.print(i);

			VRMLFieldDeclaration decl = node.getFieldDeclaration(i);
			if (decl != null)
			{
//				String fieldName = decl.getName();	// field name
//				System.out.println(" "+fieldName);

				int fieldAccess = decl.getAccessType();
				if ((fieldAccess == FieldConstants.FIELD) || (fieldAccess == FieldConstants.EXPOSEDFIELD))
				{
					nValidFields++;
				}
			}
		}

		if (nValidFields <= 0) return null;
		String[] fieldNames = new String[nValidFields];
		
		int iField = 0;
		for (i=1; i<n; i++)
		{
//			System.out.print(indentStr);
//			System.out.print(i);

			VRMLFieldDeclaration decl = node.getFieldDeclaration(i);
			if (decl != null)
			{
				String fieldName = decl.getName();	// field name
//				System.out.println(" "+fieldName);

				int fieldAccess = decl.getAccessType();
				if ((fieldAccess == FieldConstants.FIELD) || (fieldAccess == FieldConstants.EXPOSEDFIELD))
				{
					fieldNames[iField] = fieldName;
					iField++;
				}
			}
		}
		
		return fieldNames;
	}
	
	// ---------------------------------------------
	//  get field value of node
	// ---------------------------------------------
	public VRMLFieldData getFieldValue(VRMLNode node, String fieldName)
	{
		int i = node.getFieldIndex(fieldName);
		VRMLFieldData data = ((VRMLNodeType)node).getFieldValue(i);
		return data;
	}
	
	// ---------------------------------------------
	//  check if specified field is SFNode
	// ---------------------------------------------
	public boolean isSFNodeField(VRMLNode node, String fieldName)
	{
		int i = node.getFieldIndex(fieldName);
		VRMLFieldData data = ((VRMLNodeType)node).getFieldValue(i);

		return (data.dataType == VRMLFieldData.NODE_DATA);
	}
	
	// ---------------------------------------------
	//  check if specified field is MFNode
	// ---------------------------------------------
	public boolean isMFNodeField(VRMLNode node, String fieldName)
	{
		int i = node.getFieldIndex(fieldName);
		VRMLFieldData data = ((VRMLNodeType)node).getFieldValue(i);

		return (data.dataType == VRMLFieldData.NODE_ARRAY_DATA);
	}
	
	// ---------------------------------------------
	//  get all def names
	// ---------------------------------------------
	public String[] getDefNames()
	{
		Map<String, VRMLNodeType> m = handler.getDefMap();
		
		Set<String> strSet = m.keySet();
		String[] ret = new String[strSet.size()];
		
		int i=0;
		Iterator<String> itr = strSet.iterator();
		while (itr.hasNext())
		{
			String defName = itr.next();
			ret[i] = defName;
			i++;
		}
		
		return ret;
	}
	
	// ---------------------------------------------
	//  get def node
	// ---------------------------------------------
	public VRMLNode getDefNode(String defName)
	{
		Map<String, VRMLNodeType> m = handler.getDefMap();
		
		VRMLNode node = (VRMLNode)m.get(defName);
		
		return node;
	}
}

