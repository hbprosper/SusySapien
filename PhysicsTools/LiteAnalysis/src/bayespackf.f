      subroutine bayespackopen(filename)
      character*(*) filename
      integer lun
      parameter (lun=55)
      integer lf
***************************************************************
      lf = len(filename)
      open(unit=lun, file=filename(1:lf), status='UNKNOWN')
      write(lun,'(''Filename: '',A)') filename(1:lf)
      return

      entry bayespackclose()
      close(lun)
      return
      end
