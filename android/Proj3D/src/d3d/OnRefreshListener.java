package d3d;

import javax.microedition.khronos.opengles.GL10;

import com.threed.jpct.World;

public interface OnRefreshListener {
	public void BeforeRender(GL10 gl);
	public void AfterRender(GL10 gl);
	public void onSurfaceChanged(World world);
}
