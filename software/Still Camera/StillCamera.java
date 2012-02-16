public class StillCamera implements StillCameraInterface
{
  private enum Mode {IDLE, CAPTURE_JPG, CAPTURE_AVI, PLAYBACK}
  private Mode mode;
}