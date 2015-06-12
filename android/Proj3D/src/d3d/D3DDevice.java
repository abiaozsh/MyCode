package d3d;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import com.threed.jpct.FrameBuffer;
import com.threed.jpct.Light;
import com.threed.jpct.RGBColor;
import com.threed.jpct.SimpleVector;
import com.threed.jpct.World;
import com.threed.jpct.util.MemoryHelper;

public class D3DDevice extends GLSurfaceView implements Renderer {

	OnRefreshListener onRefreshListener;
	Activity activity;
	private FrameBuffer fb = null;
	public World world = null;
	private RGBColor back = new RGBColor(50, 50, 100);

	public D3DDevice(Activity activity, OnRefreshListener onRefreshListener) {
		super(activity.getApplication());
		this.activity = activity;
		setEGLContextClientVersion(2);
		setRenderer(this);
		activity.setContentView(this);
		this.onRefreshListener = onRefreshListener;
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {

	}

	public void onDrawFrame(GL10 gl) {
		onRefreshListener.BeforeRender(gl);
		fb.clear(back);
		world.renderScene(fb);
		world.draw(fb);
		fb.display();
		onRefreshListener.AfterRender(gl);
	}

	private Light sun = null;

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		try {
			if (fb != null) {
				fb.dispose();
			}
			fb = new FrameBuffer(w, h);

			world = new World();
			world.setAmbientLight(20, 20, 20);

			sun = new Light(world);
			sun.setIntensity(250, 250, 250);

			sun.setPosition(SimpleVector.create(100, 100, 100));

			onRefreshListener.onSurfaceChanged(world);

			MemoryHelper.compact();
		} catch (Throwable t) {
			t.printStackTrace();
		}
	}


}
