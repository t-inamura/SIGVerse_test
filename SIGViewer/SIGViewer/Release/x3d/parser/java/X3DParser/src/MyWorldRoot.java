import org.web3d.vrml.nodes.VRMLNodeType;
import org.web3d.vrml.renderer.common.nodes.core.BaseWorldRoot;
import org.web3d.vrml.renderer.norender.nodes.NRVRMLNode;

public class MyWorldRoot extends BaseWorldRoot implements NRVRMLNode {

	public MyWorldRoot() {
	}
	
    protected void addChildNode(VRMLNodeType node) {
        super.addChildNode(node);
    }    
}
